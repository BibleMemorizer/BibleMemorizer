TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= warn_on qt

HEADERS	+= core/bibleplugin.h \
	core/biblepluginmeta.h \
	core/bmemversion.h \
	core/correctionword.h \
	core/filterallowall.h \
	core/filterand.h \
	core/filtercategory.h \
	core/filterinvalid.h \
	core/filterlist.h \
	core/filternot.h \
	core/filteror.h \
	core/filterquery.h \
	core/filtersearchatom.h \
	core/filtertestament.h \
	core/help.h \
	core/searchfilter.h \
	core/settings.h \
	core/ubook.h \
	core/verse.h \
	core/versecollection.h \
	ui/aboutact.h \
	ui/mainact.h \
	ui/plugininfoact.h \
	ui/preferencesact.h \
	ui/quizact.h \
	ui/quizcreateact.h \
	ui/quizmultiact.h \
	ui/quizrefact.h \
	ui/verseact.h \
	ui/verseselectact.h

SOURCES	+= biblememorizer.cpp \
	core/bibleplugin.cpp \
	core/biblepluginmeta.cpp \
	core/correctionword.cpp \
	core/filterallowall.cpp \
	core/filterand.cpp \
	core/filtercategory.cpp \
	core/filterinvalid.cpp \
	core/filterlist.cpp \
	core/filternot.cpp \
	core/filteror.cpp \
	core/filterquery.cpp \
	core/filtersearchatom.cpp \
	core/filtertestament.cpp \
	core/help.cpp \
	core/searchfilter.cpp \
	core/settings.cpp \
	core/verse.cpp \
	core/versecollection.cpp \
	ui/aboutact.cpp \
	ui/mainact.cpp \
	ui/plugininfoact.cpp \
	ui/preferencesact.cpp \
	ui/quizact.cpp \
	ui/quizcreateact.cpp \
	ui/quizmultiact.cpp \
	ui/quizrefact.cpp \
	ui/verseact.cpp \
	ui/verseselectact.cpp

FORMS	= ui/AboutUI.ui \
	ui/MainUI.ui \
	ui/PluginInfoUI.ui \
	ui/PreferencesUI.ui \
	ui/QuizCreateUI.ui \
	ui/QuizRefUI.ui \
	ui/QuizUI.ui \
	ui/VerseUI.ui \
	ui/VerseSelectUI.ui

# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/biblememorizer
include(../qmake_vars)


unix {
    INSTALLS += desktop icons16 icons22 icons32 icons48 icons64 icons128
    desktop.path = $${MAIN_PATH}/share/applications
    desktop.files = ../biblememorizer.desktop
    ICON_ROOT = $${MAIN_PATH}/share/icons/hicolor
    icons16.path = $${ICON_ROOT}/16x16/apps
    icons16.files = ../icons/build/16x16/apps/biblememorizer.png
    icons22.path = $${ICON_ROOT}/22x22/apps
    icons22.files = ../icons/build/22x22/apps/biblememorizer.png
    icons32.path = $${ICON_ROOT}/32x32/apps
    icons32.files = ../icons/build/32x32/apps/biblememorizer.png
    icons48.path = $${ICON_ROOT}/48x48/apps
    icons48.files = ../icons/build/48x48/apps/biblememorizer.png
    icons64.path = $${ICON_ROOT}/64x64/apps
    icons64.files = ../icons/build/64x64/apps/biblememorizer.png
    icons128.path = $${ICON_ROOT}/128x128/apps
    icons128.files = ../icons/build/128x128/apps/biblememorizer.png
}

TARGET = ../bin/biblememorizer

mac {
    TARGET = ../bin/BibleMemorizer
    ICON = ../icons/BibleMemorizer.icns
}


unix {
    target.path = $${MAIN_PATH}\bin
}
mac {
    target.path = $${MAIN_PATH}
    INSTALLS -= desktop icons16 icons22 icons32 icons48 icons64 icons128
}
win32{
    target.path = $${MAIN_PATH}
    RC_FILE = ../icons/biblememorizer.rc
}

INSTALLS += target
QT += xml \
      widgets
