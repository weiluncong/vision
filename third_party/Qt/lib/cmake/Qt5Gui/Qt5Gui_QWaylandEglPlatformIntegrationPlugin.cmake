
add_library(Qt5::QWaylandEglPlatformIntegrationPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QWaylandEglPlatformIntegrationPlugin RELEASE "platforms/libqwayland-egl.so")

list(APPEND Qt5Gui_PLUGINS Qt5::QWaylandEglPlatformIntegrationPlugin)
