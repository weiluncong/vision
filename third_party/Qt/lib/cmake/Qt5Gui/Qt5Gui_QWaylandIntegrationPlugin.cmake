
add_library(Qt5::QWaylandIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QWaylandIntegrationPlugin RELEASE "platforms/libqwayland-generic.so")

list(APPEND Qt5Gui_PLUGINS Qt5::QWaylandIntegrationPlugin)
