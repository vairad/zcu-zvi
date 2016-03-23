#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T10:15:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KonvolutionFinder
TEMPLATE = app

INCLUDEPATH += D:\Programy\OpenCV\opencv\build\include
LIBS += -lopencv_core -lopencv_flann -lopencv_highgui

SOURCES += main.cpp\
        mainwindow.cpp \
    core/cleaner.cpp

HEADERS  += mainwindow.h \
    core/cleaner.h

FORMS    += mainwindow.ui
