
add_library(Qt5::QWaylandFullScreenShellV1IntegrationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandFullScreenShellV1IntegrationPlugin RELEASE "wayland-shell-integration/libfullscreen-shell-v1.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandFullScreenShellV1IntegrationPlugin)
