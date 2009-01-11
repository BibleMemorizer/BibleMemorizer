include(../plugin_vars)
INSTALLS += target
target.path = $${PLUGIN_DIR}
meta.path = $${PLUGIN_DIR}
meta.files = lang_en.plugin
INSTALLS += meta
authors.path = $${PLUGIN_DIR}
authors.files = authors.lang_en
INSTALLS += authors
license.path = $${PLUGIN_DIR}
license.files = license.lang_en.html
INSTALLS += license
HEADERS += ../bmemplugin.h 
SOURCES += lang_en.cpp 
CONFIG += warn_on \
plugin
CONFIG -= qt
TEMPLATE = lib
