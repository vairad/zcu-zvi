#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T10:15:41
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KonvolutionFinder
TEMPLATE = app

CONFIG += c++11

unix {
LIBS += -lopencv_core -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_objdetect
}

win32{
# replace with your paths to opencv
INCLUDEPATH += D:\Programy\OpenCV\opencv\build\include
LIBS += -LD:\Programy\OpenCV\opencv\build\x64\vc12\lib
LIBS += -lopencv_core2412d -lopencv_flann2412d -lopencv_highgui2412d -lopencv_imgproc2412d -lopencv_objdetect2412d
}

SOURCES += main.cpp\
        gui/mainwindow.cpp \
        core/cleaner.cpp \
        core/filename_factory.cpp \
        core/exception.cpp \
        test/testclass.cpp \
        core/histogrammodifier.cpp \
    core/convolutiondescriptor.cpp \
    gui/descriptordialog.cpp \
    core/haarfinder.cpp \
    gui/helpwindow.cpp


HEADERS  += gui/mainwindow.h \
            core/cleaner.h \
            core/filename_factory.h \
            core/exception.h \
            test/testclass.h \
            core/histogrammodifier.h \
            core/convolutiondescriptor.h \
            gui/descriptordialog.h \
            core/haarfinder.h \
            gui/helpwindow.h

FORMS    += gui/mainwindow.ui
