# -------------------------------------------------
# Project created by QtCreator 2009-11-10T18:31:00
# -------------------------------------------------
QT += network \
    webkit \
    xml
TARGET = qfacebookconnect

TEMPLATE = lib
DESTDIR = ./debug
DEFINES += QFACEBOOKCONNECT_LIBRARY
INCLUDEPATH = inc/
SOURCES += src/fbsession.cpp
SOURCES += src/fbrequest.cpp
SOURCES += src/fbxmlhandler.cpp
SOURCES += src/fbdialog.cpp
SOURCES += src/fblogindialog.cpp
SOURCES += src/fbpermissiondialog.cpp
SOURCES += src/fbstreamdialog.cpp
SOURCES += src/fbfeeddialog.cpp
HEADERS += inc/fbconnectglobal.h
HEADERS += inc/fbsession.h
HEADERS += inc/fbrequest.h
HEADERS += inc/fbxmlhandler.h
HEADERS += inc/fberror.h
HEADERS += inc/fbdialog.h
HEADERS += inc/fblogindialog.h
HEADERS += inc/fbpermissiondialog.h
HEADERS += inc/fbstreamdialog.h
HEADERS += inc/fbfeeddialog.h
RESOURCES += resource/qfbconnectimages.qrc

symbian: {
INCLUDEPATH += /epoc32/include/stdapis
TARGET.EPOCALLOWDLLDATA = 1 # Symbian specific
TARGET.UID3 = 0xA89FA521		# Symbian specific, unprotected uid3 from symbiansigned.com
TARGET.CAPABILITY = NetworkServices # Symbian specific, we just need network access
}
