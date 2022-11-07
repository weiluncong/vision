
add_library(Qt5::ShmServerBufferIntegrationPlugin MODULE IMPORTED)

_populate_WaylandCompositor_plugin_properties(ShmServerBufferIntegrationPlugin RELEASE "wayland-graphics-integration-server/libshm-emulation-server.so")

list(APPEND Qt5WaylandCompositor_PLUGINS Qt5::ShmServerBufferIntegrationPlugin)
