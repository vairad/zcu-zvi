#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T10:15:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KonvolutionFinder
TEMPLATE = app


unix {
LIBS += -lopencv_core -lopencv_flann -lopencv_highgui
}

win32{
INCLUDEPATH += D:\Programy\OpenCV\opencv\build\include
}

SOURCES += main.cpp\
        mainwindow.cpp \
        core/cleaner.cpp \
        core/filename_factory.cpp

HEADERS  += mainwindow.h \
            core/cleaner.h \
            core/filename_factory.h

FORMS    += mainwindow.ui
