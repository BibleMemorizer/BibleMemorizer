include(../plugin_vars)
INSTALLS += target
target.path = $${PLUGIN_DIR}
meta.path = $${PLUGIN_DIR}
meta.files = crosswire.plugin
INSTALLS += meta
authors.path = $${PLUGIN_DIR}
authors.files = authors.crosswire
INSTALLS += authors
license.path = $${PLUGIN_DIR}
license.files = license.crosswire.html
INSTALLS += license
HEADERS += ../bmemplugin.h \
           btstringmgr.h
SOURCES += crosswire.cpp \
           btstringmgr.cpp
INCLUDEPATH += $${SWORD_PATH}
LIBS += $${SWORD_LIB}
CONFIG += warn_on \
plugin
CONFIG += qt
TEMPLATE = lib
