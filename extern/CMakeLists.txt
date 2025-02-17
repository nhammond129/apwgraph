## cmake submodule stuff
# via https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/imgui/imgui.cpp")
    message(FATAL_ERROR "Submodule imgui not found, please run 'git submodule update --init --recursive'")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/SDL/CMakeLists.txt")
    message(FATAL_ERROR "Submodule SDL not found, please run 'git submodule update --init --recursive'")
endif()


# glob imgui sources from extern/imgui/
file(GLOB IMGUI_SOURCES
    "imgui/*.cpp"
    "imgui/backends/imgui_impl_sdl3.cpp"
    "imgui/backends/imgui_impl_sdlrenderer3.cpp"
    # "extern/imgui/backends/imgui_impl_opengl3.cpp"
)
file(GLOB_RECURSE IMGUI_HEADERS
    "imgui/*.h"
    "imgui/backends/imgui_impl_sdl3.h"
    "imgui/backends/imgui_impl_sdlrenderer3.h"
    # "extern/imgui/backends/imgui_impl_opengl3.h"
)

# add imgui library
add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui
    PUBLIC
        "imgui/"
        "imgui/backends/"
)