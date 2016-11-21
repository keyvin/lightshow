#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T17:36:34
#
#-------------------------------------------------

QT      += core gui
QT      += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lightshow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pixel.cpp \
    qpixel.cpp \
    program.cpp \
    serializer.cpp

HEADERS  += mainwindow.h \
    pixel.h \
    qpixel.h \
    program.h \
    serializer.h

FORMS    += mainwindow.ui
