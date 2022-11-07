
add_library(Qt5::DmaBufServerBufferPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(DmaBufServerBufferPlugin RELEASE "wayland-graphics-integration-client/libdmabuf-server.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::DmaBufServerBufferPlugin)
