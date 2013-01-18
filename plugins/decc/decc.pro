#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T19:33:11
#
#-------------------------------------------------

QT       += gui
TEMPLATE = lib
CONFIG += plugin
VERSION = 1.0.1
PLUGIN_NAME=decc

GENERATED_DIR = ../../generated/plugin/decc
# Use common project definitions.
include(../../common.pro)

# For plugins
INCLUDEPATH    += ../../librecad/src/plugins

# For Google V8 JavaScript -engine stuff.
INCLUDEPATH += /home/halftan/src/V8/v8/include
DEPENDPATH += /home/halftan/src/V8/v8/include



SOURCES += decc.cpp \
    dialogdecc.cpp \
    script/v8base.cpp \
    script/scriptdecc.cpp \
    cad.cpp

HEADERS += decc.h \
    dialogdecc.h \
    script/v8base.h \
    script/scriptdecc.h \
    cad.h


# DLLDESTDIR = ../../unix/resources/plugins/
win32 {
        DLLDESTDIR = ../../windows/resources/plugins
        TARGET = $$PLUGIN_NAME
}
unix {
    macx { 
	TARGET = ../../LibreCAD.app/Contents/Resources/plugins/$$PLUGIN_NAME
    }
    else { 
	TARGET = ../../unix/resources/plugins/$$PLUGIN_NAME
    }
}

FORMS += \
    dialogdecc.ui

OTHER_FILES += \
    design.txt

