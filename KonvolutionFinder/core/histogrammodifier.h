#ifndef HISTOGRAMMODIFIER_H
#define HISTOGRAMMODIFIER_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class HistogramModifier
{
    double high_bright;
    double low_bright;

    double bright_size; // (high_bright - low_bright)

    double alpha;
    int beta;

    unsigned char roundBright(double value);

public:
    HistogramModifier(double high_bright, double low_bright);
    ~HistogramModifier();

    bool setAlpha(double alpha);
    bool setBeta(int beta);

    void moveHistogram(cv::Mat *imageToChange, cv::Mat *outputImage);
    void stretchHistogram(cv::Mat *imageToChange, cv::Mat *outputImage, double low, double high);
};

#endif // HISTOGRAMMODIFIER_H
