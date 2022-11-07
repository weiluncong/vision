
add_library(Qt5::ShmServerBufferPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(ShmServerBufferPlugin RELEASE "wayland-graphics-integration-client/libshm-emulation-server.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::ShmServerBufferPlugin)
