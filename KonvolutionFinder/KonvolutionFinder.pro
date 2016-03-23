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
LIBS += -LD:\Programy\OpenCV\opencv\build\x86\vc11\lib
LIBS += -lopencv_core2412 -lopencv_flann2412 -lopencv_highgui2412
}

SOURCES += main.cpp\
        mainwindow.cpp \
    core/cleaner.cpp \
    core/filename_factory.cpp

HEADERS  += mainwindow.h \
    core/cleaner.h \
    core/filename_factory.h

FORMS    += mainwindow.ui
