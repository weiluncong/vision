
add_library(Qt5::QWaylandWlShellIntegrationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandWlShellIntegrationPlugin RELEASE "wayland-shell-integration/libwl-shell.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandWlShellIntegrationPlugin)
