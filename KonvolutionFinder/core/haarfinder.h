#ifndef HAARFINDER_H
#define HAARFINDER_H

#include <string>
#include <iostream>

#include <QThread>
#include <QImage>
#include <QObject>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "core/filename_factory.h"
#include "core/exception.h"
#include "core/histogrammodifier.h"

class HaarFinder : public QThread {
    Q_OBJECT
    FilenameFactory *names;
    cv::CascadeClassifier cascade;
    std::string cascade_name;

    std::vector<cv::Rect> detectInclusions( cv::Mat frame );

    void drawInclusions(std::vector<cv::Rect> *inclusions, cv::Mat *image);
public:
    HaarFinder(FilenameFactory *names, std::string inclusionCascadeName);

    virtual void run();
    void cvMatToQImage(cv::Mat *input, int destination);
signals:
    void showImage(QImage *image, int destination);
    void imagesProcessed(unsigned int count);
};

#endif // HAARFINDER_H
