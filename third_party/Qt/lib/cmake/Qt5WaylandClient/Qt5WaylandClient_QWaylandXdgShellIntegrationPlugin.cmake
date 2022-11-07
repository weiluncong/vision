
add_library(Qt5::QWaylandXdgShellIntegrationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandXdgShellIntegrationPlugin RELEASE "wayland-shell-integration/libxdg-shell.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandXdgShellIntegrationPlugin)
