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

    int counter = 0;

    virtual void run();

    void cvMatToQImage(cv::Mat *input, int destination);
public:
    void setFactory(FilenameFactory *names);
signals:
    void showImage(QImage *image, int destination);
};

#endif // CLEANER_H
