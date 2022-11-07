
add_library(Qt5::QTextToSpeechPluginSpeechd MODULE IMPORTED)

_populate_TextToSpeech_plugin_properties(QTextToSpeechPluginSpeechd RELEASE "texttospeech/libqtexttospeech_speechd.so")

list(APPEND Qt5TextToSpeech_PLUGINS Qt5::QTextToSpeechPluginSpeechd)
