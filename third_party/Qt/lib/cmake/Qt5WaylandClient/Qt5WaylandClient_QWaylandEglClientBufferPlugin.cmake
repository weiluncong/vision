
add_library(Qt5::QWaylandEglClientBufferPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandEglClientBufferPlugin RELEASE "wayland-graphics-integration-client/libqt-plugin-wayland-egl.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandEglClientBufferPlugin)
