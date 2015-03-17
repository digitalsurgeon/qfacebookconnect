#-------------------------------------------------
#
# Project created by QtCreator 2009-11-20T00:54:21
#
#-------------------------------------------------

QT       += network webkit xml

TARGET = sample
TEMPLATE = app
DESTDIR = ../debug

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


INCLUDEPATH = ../inc/
win32:LIBS += ../debug/libqfacebookconnect.a
unix:LIBS += ../debug/libqfacebookconnect.so

symbian: {
	TARGET.CAPABILITY = NetworkServices # Symbian specific, we just need network access
	TARGET.UID3 = 0xA89FA522 # Symbian specific, unprotected uid3 from symbiansigned.com

	INCLUDEPATH += /epoc32/include/stdapis # this seems to be a bug.
	LIBS += -lqfacebookconnect

	libqfacebookconnect.sources = qfacebookconnect.dll
	libqfacebookconnect.path  = \sys\bin
	DEPLOYMENT += libqfacebookconnect
	}

ICON = gfx/fb.svg

