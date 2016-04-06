#ifndef CLEANER_H
#define CLEANER_H

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QImage>
#include <QThread>

#include "core/filename_factory.h"

class Cleaner : public QThread {
    Q_OBJECT
    FilenameFactory *names;

    int counter;


    int lowThreshold;
    int ratio;
    int kernelSize;

    virtual void run();

    void cannyEdges(cv::Mat src, cv::Mat out);
    void morphologyClose(cv::Mat src, cv::Mat out);

    void cvMatToQImage(cv::Mat *input, int destination);
    void convexHull(cv::Mat input, std::vector<std::vector<cv::Point> > &out);



public:
    void setFactory(FilenameFactory *names);
signals:
    void showImage(QImage *image, int destination);
public slots:
    void setLowThresh(int value);
    void setRatio(int value);
    void setKenelSize(int value);
};

#endif // CLEANER_H
