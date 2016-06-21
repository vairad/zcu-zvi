#ifndef CLEANER_H
#define CLEANER_H

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QImage>
#include <QThread>

#include "core/filename_factory.h"
#include "core/convolutiondescriptor.h"


class Cleaner : public QThread {
    Q_OBJECT
    FilenameFactory *names;
    ConvolutionDescriptor *descriptor;

    int counter;

    const cv::Scalar CONTOUR_COLOR = cv::Scalar(20, 20, 255);
    const cv::Scalar CONTOUR_COLOR_II = cv::Scalar(255, 0, 0);

    int threshold;
    int lowThreshold;
    int ratio;
    int kernelSize;

    virtual void run();

    void cannyEdges(cv::Mat src, cv::Mat out);
    void morphologyClose(cv::Mat src, cv::Mat out);

    void cvMatToQImage(cv::Mat *input, int destination);
    void convexHull(cv::Mat input, std::vector<std::vector<cv::Point> > &out);
    void fillAllContours(cv::Mat input, const std::vector<std::vector<cv::Point> > &contours);

    bool checkContour(std::vector<cv::Point> contour);

public:
    static int defaultThreshold;

    Cleaner(FilenameFactory *names, ConvolutionDescriptor *descriptor);
signals:
    void showImage(QImage *image, int destination);
public slots:
    void setThresh(int value);
    void setLowThresh(int value);
    void setRatio(int value);
    void setKernelSize(int value);
};

#endif // CLEANER_H
