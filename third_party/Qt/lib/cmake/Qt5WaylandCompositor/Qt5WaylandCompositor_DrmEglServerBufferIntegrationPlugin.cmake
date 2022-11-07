
add_library(Qt5::DrmEglServerBufferIntegrationPlugin MODULE IMPORTED)

_populate_WaylandCompositor_plugin_properties(DrmEglServerBufferIntegrationPlugin RELEASE "wayland-graphics-integration-server/libdrm-egl-server.so")

list(APPEND Qt5WaylandCompositor_PLUGINS Qt5::DrmEglServerBufferIntegrationPlugin)
