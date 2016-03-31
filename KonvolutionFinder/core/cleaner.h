#ifndef CLEANER_H
#define CLEANER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QImage>
#include <QThread>

#include "core/filename_factory.h"

class Cleaner : public QThread {
    Q_OBJECT
    FilenameFactory *names;
    virtual void run();

    void cvMatToQImage(cv::Mat *input);
public:
    void setFactory(FilenameFactory *names);
    void run2();
signals:
    void showImage(QImage *image);
};

#endif // CLEANER_H
