SOURCES += main.cpp \
pokerClock.cpp \
prizeWindow.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/pokerclock

QT -= gui
HEADERS += pokerclock.h \
pokerClock.h \
powerclock.h \
prizeWindow.h
FORMS += pokerClock.ui \
prizeWindow.ui
