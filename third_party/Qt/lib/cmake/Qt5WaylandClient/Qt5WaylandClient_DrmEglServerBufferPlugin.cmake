
add_library(Qt5::DrmEglServerBufferPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(DrmEglServerBufferPlugin RELEASE "wayland-graphics-integration-client/libdrm-egl-server.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::DrmEglServerBufferPlugin)
