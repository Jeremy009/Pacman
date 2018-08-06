#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T13:56:25
#
#-------------------------------------------------

QT       += core gui multimedia
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pacman
TEMPLATE = app


SOURCES += main.cpp \
    Game.cpp \
    Level.cpp \
    Tile.cpp \
    Score.cpp \
    Health.cpp \
    MovingObjects.cpp

HEADERS  += \
    Game.h \
    Level.h \
    Tile.h \
    Score.h \
    Health.h \
    MovingObjects.h

FORMS    +=

RESOURCES += \
    resources.qrc
