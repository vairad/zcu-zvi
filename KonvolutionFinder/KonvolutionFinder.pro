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
LIBS += -LD:\Programy\OpenCV\opencv\build\x64\vc12\lib
LIBS += -lopencv_core2412d -lopencv_flann2412d -lopencv_highgui2412d
}

SOURCES += main.cpp\
        mainwindow.cpp \
        core/cleaner.cpp \
        core/filename_factory.cpp \
        core/exception.cpp \
        test/testclass.cpp


HEADERS  += mainwindow.h \
            core/cleaner.h \
            core/filename_factory.h \
            core/exception.h \
            test/testclass.h

FORMS    += mainwindow.ui
