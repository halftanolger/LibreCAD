#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T08:51:24
#
#-------------------------------------------------

QT       -= gui

TARGET = jdxfrw
TEMPLATE = lib

DEFINES += J_FILTERDXFRW_LIBRARY

SOURCES += j_filterdxfrw.cpp \
    Jlibdxfrw.cpp

HEADERS += j_filterdxfrw.h\
        j_filterdxfrw_global.h \
    Jlibdxfrw.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    Jlibdxfrw.java \
    howto.txt \
    LineData.java



INCLUDEPATH += /usr/lib/jvm/java-6-openjdk-amd64/include/
INCLUDEPATH += /usr/lib/jvm/java-6-openjdk-amd64/include/linux/
LIBS+= -L/usr/lib/jvm/java-6-openjdk-amd64/jre/lib/amd64/


# Additional libraries to load
LIBS += -L../../generated/lib  \
    -ldxfrw \


INCLUDEPATH += \
    ../../libraries/libdxfrw/src \
    /usr/local/Trolltech/Qt-4.8.4/include/QtGui/ \
    ../../librecad/src/lib/engine


