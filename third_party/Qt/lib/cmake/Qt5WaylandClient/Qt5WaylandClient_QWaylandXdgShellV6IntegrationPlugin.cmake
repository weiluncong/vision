
add_library(Qt5::QWaylandXdgShellV6IntegrationPlugin MODULE IMPORTED)

_populate_WaylandClient_plugin_properties(QWaylandXdgShellV6IntegrationPlugin RELEASE "wayland-shell-integration/libxdg-shell-v6.so")

list(APPEND Qt5WaylandClient_PLUGINS Qt5::QWaylandXdgShellV6IntegrationPlugin)
