
add_library(Qt5::QWaylandBradientDecorationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandBradientDecorationPlugin RELEASE "wayland-decoration-client/libbradient.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandBradientDecorationPlugin)
