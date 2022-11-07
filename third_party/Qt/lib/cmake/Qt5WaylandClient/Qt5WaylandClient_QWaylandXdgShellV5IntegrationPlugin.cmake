
add_library(Qt5::QWaylandXdgShellV5IntegrationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandXdgShellV5IntegrationPlugin RELEASE "wayland-shell-integration/libxdg-shell-v5.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandXdgShellV5IntegrationPlugin)
