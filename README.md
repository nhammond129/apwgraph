# apwgraph - A PipeWire Patchbay Interface
Inspired by [`qpwgraph` - 'A PipeWire Graph Qt GUI Interface'](https://github.com/rncbc/qpwgraph).

I love the utility and concept of `qpwgraph` but I have some mild gripes with the interface, so I'm forking the idea and ruining it with my own preferences.


## Progress
[x] slap together dependencies
[ ] do a pipewire
  [x] connect to daemon
  [x] enumerate objects and get events
  [ ] save intermediate state and dump to GUI
[ ] make ui do things
  [ ] make/break connections

## Stretch Goals
- throw it on the AUR
- make it cuter
  - gotta have cute volume knobs. gotta.
  - live waveform display toggle?
- make it less jank


## notes-to-self
[pw docs](https://docs.pipewire.org/)
[`pw-cli` source](https://github.com/PipeWire/pipewire/blob/master/src/tools/pw-cli.c)
[imgui 'useful extensions'](https://github.com/ocornut/imgui/wiki/Useful-Extensions)
  [thedmd/imgui-node-editor](https://github.com/thedmd/imgui-node-editor)
