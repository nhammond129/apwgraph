#pragma once

#include <pipewire/pipewire.h>
#include <cstdio>
#include <cstring>
#include <memory>

class PipewireInterface {
private:
    static std::unique_ptr<PipewireInterface> _instance;

    struct pw_data {
        struct pw_main_loop *loop;
        struct pw_context *context;
        struct pw_core *core;

        struct pw_registry *registry;
        struct spa_hook registry_listener;

        struct pw_client *client;
        struct spa_hook client_listener;
    } data;

    static void registry_event_global(void* data, uint32_t id, uint32_t permissions, const char* type, uint32_t version, const struct spa_dict* props) {
        static const struct pw_client_events client_events = {
            .version = PW_VERSION_CLIENT_EVENTS,
            .info = client_info,
        };

        printf("global: %u %s/%d\n", id, type, version);

        printf("binding ...\n");
        struct pw_data* d = static_cast<pw_data*>(data);
        if (data->client == nullptr) return;

        if (strcmp(type, PW_TYPE_INTERFACE_Client) == 0) {
            data->client = pw_registry_bind(data->registry, id, type, PW_VERSION_CLIENT, 0);
            pw_client_add_listener(data->client, &data->client_listener, &client_events, data);  // should we save the returned spa_hook and remove later???
        }
    }

    static void registry_event_global_remove(void* data, uint32_t id) {
        printf("global remove: %u\n", id);
    }

    PipewireInterface() {
        if (_instance != nullptr) {
            throw std::runtime_error("Tried to create a second instance of PipewireInterface");
        }
        pw_init(nullptr, nullptr);  // argc, argv
        data.loop = pw_main_loop_new(nullptr);
        data.context = pw_context_new(pw_main_loop_get_loop(loop), nullptr, 0);
        data.core = pw_context_connect(context, nullptr, 0);
        data.registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

        fprintf(stdout, "Compiled with pipewire %s\n", pw_get_headers_version());
        fprintf(stdout, "Linked with pipewire %s\n\n", pw_get_library_version());
    }

    static void client_info(void* object, const struct pw_client_info* info) {
        struct pw_data* data = static_cast<pw_data*>(object);
        const struct spa_dict_item* item;

        printf("client: id:%u\n", info->id);
        printf("\tprops:\n");
        spa_dict_for_each(item, info->props) {
            printf("\t\t%s = '%s'\n", item->key, item->value);
        }
    }

    struct roundtrip_data {
        int pending;
        struct pw_main_loop* loop;
    };

    static void on_core_done(void* data, uint32_t id, int seq) {
        struct roundtrip_data* d = static_cast<roundtrip_data*>(data);

        if (id == PW_ID_CORE && seq == d->pending) {
            pw_main_loop_quit(d->loop);
        }
    }

    void roundtrip(struct pw_data* state) {
        static const struct pw_registry_events registry_events = {
            .version = PW_VERSION_REGISTRY_EVENTS,
            .global = registry_event_global,
        };
        static const struct pw_core_events core_events = {
            .version = PW_VERSION_CORE_EVENTS,
            .done = on_core_done,
        };

        struct roundtrip_data d = { .loop = state.loop };
        struct spa_hook registry_listener;
        struct spa_hook core_listener;
        int err;

        // spa_zero(registry_listener);  // not needed?

        pw_registry_add_listener(registry, &registry_listener, &registry_events, nullptr);
        pw_core_add_listener(core, &core_listener, &core_events, &d);

        d.pending = pw_core_sync(core, PW_ID_CORE, 0);

        if ((err = pw_main_loop_run(loop)) < 0)
            fprintf(stderr, "pw_main_loop_run error: %s\n", strerror(err));

        // cleanup
        spa_hook_remove(&core_listener);
        spa_hook_remove(&registry_listener);
    }

public:
    static PipewireInterface& get_instance() {
        if (_instance == nullptr) {
            _instance = std::make_unique<PipewireInterface>();
        }
        return *_instance;
    }

    static void destroy_instance() {
        _instance.reset();
    }

    void poll() {
        roundtrip(data);
    }

    ~PipewireInterface() {
        // pw cleanup
        pw_proxy_destroy((struct pw_proxy*) data.registry);
        pw_core_disconnect(data.core);
        pw_context_destroy(data.context);
        pw_main_loop_destroy(data.loop);
    }
};