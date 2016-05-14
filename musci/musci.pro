#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T16:31:14
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = musci
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mylrc.cpp

HEADERS  += widget.h \
    mylrc.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc
