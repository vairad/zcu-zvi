#ifndef StructuralFinder_H
#define StructuralFinder_H

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QImage>
#include <QThread>

#include "core/exception.h"
#include "core/histogrammodifier.h"
#include "core/filename_factory.h"
#include "core/convolutiondescriptor.h"


class StructuralFinder : public QThread {
    Q_OBJECT
    FilenameFactory *names;
    ConvolutionDescriptor *descriptor;

    const cv::Scalar CONTOUR_COLOR = cv::Scalar(20, 20, 255);
    const cv::Scalar CONTOUR_COLOR_II = cv::Scalar(255, 0, 0);

    int threshold;

    virtual void run();

    void cannyEdges(cv::Mat src, cv::Mat out);

    void cvMatToQImage(cv::Mat *input, int destination);

    bool checkContour(std::vector<cv::Point> contour);

    bool checkAspectRatio(int width, int height);
    bool checkMinVerticies(unsigned int verticies);
    bool checkMaxVerticies(unsigned int verticies);
    bool checkExtent(double contourArea, double rectArea);
public:
    static int defaultThreshold;

    StructuralFinder(FilenameFactory *names, ConvolutionDescriptor *descriptor, int threshold);
signals:
    void showImage(QImage *image, int destination);
    void imagesProcessed(unsigned int count);
public slots:
    void setThresh(int value);
};

#endif // StructuralFinder_H
