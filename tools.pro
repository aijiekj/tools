#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T10:21:05
#
#-------------------------------------------------

# Check if the config file exists
! include( ./common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tools
TEMPLATE = app

INCLUDEPATH += ./ \
               ../thirdparty/quazip

#include(../thirdparty/quazip/quazip.pri)

SOURCES += main.cpp\
        toools.cpp \
    zipfiles.cpp

HEADERS  += toools.h \
    zipfiles.h \
    zipfiles.h

DEFINES += QUAZIP_STATIC

FORMS    += toools.ui

DISTFILES += \
    parade.astylerc

RC_FILE  += app_info.rc
OTHER_FILES += app_info.rc


DEPENDPATH += $$PWD/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lquazipd

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libquazip.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libquazipd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/quazipd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/quazip.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libquazip.a
