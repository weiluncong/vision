
add_library(Qt5::QWaylandIviShellIntegrationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandIviShellIntegrationPlugin RELEASE "wayland-shell-integration/libivi-shell.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandIviShellIntegrationPlugin)
