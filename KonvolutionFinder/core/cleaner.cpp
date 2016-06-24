#include "core/cleaner.h"

#include <iostream>

#include "core/exception.h"
#include "core/histogrammodifier.h"


int Cleaner::defaultThreshold = 160;

Cleaner::Cleaner(FilenameFactory *names, ConvolutionDescriptor *descriptor, int threshold):  names(names), descriptor(descriptor){
    this->threshold = threshold;

    this->boolAspectRatio = true;
    this->boolExtent = true;
    this->boolMaxVerticies = true;
    this->boolMinVerticies = true;
}

void Cleaner::setThresh(int value){
    this->threshold = value;
}

/**
 * Pracovní smyčka vlákna
 * @brief Cleaner::run
 */
void Cleaner::run(){
    unsigned int counter = 0;
    cv::Mat image;
    cv::Mat originalImage;
    cv::Mat preprocessedImage;
    std::vector< std::vector<cv::Point> > hulls; // kontury nalezenych objektů.
    std::vector< std::vector<cv::Point> > potentialConvolutions; //kontury objektů splňující podmínky.
    std::vector<cv::Vec4i> hierarchy; // hierarchie nalezenych kontur


    QString name = names->getNextImageRelativePath();
    while(name != NULL){

        if(counter % 50 == 0 ){
            emit imagesProcessed(counter);
        }

        originalImage = cv::imread(name.toStdString(), CV_LOAD_IMAGE_UNCHANGED); //nacti obrazek
        cv::cvtColor(originalImage, image, CV_BGR2GRAY); // vytvor pracovni kopii (BW)

        if(!image.data){  // kontrola dat
            throw EmptyImageException(name.toStdString().c_str());
        }

        preprocessedImage = cv::Mat::zeros(image.rows, image.cols, image.type());

        HistogramModifier modifier(255, 0);
        modifier.stretchHistogram(&image, &image, 0, 150);

        cv::threshold(image, image, threshold, 255, CV_THRESH_BINARY);

        cannyEdges(image, preprocessedImage);

        cv::findContours( preprocessedImage, hulls, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,
                            cv::Point(0, 0) );

        // vykresli nalezene kontury
        for( size_t i = 0; i< hulls.size(); i++ ) {
            cv::Scalar color = CONTOUR_COLOR_II;
            cv::drawContours( originalImage, hulls, i, color, 2, 8, hierarchy, 0, cv::Point() );
        }

        //porčisti kontury podle kritérií
        for(std::vector< std::vector<cv::Point> >::iterator it = hulls.begin(); it != hulls.end(); ++it) {
            if(checkContour(*it)){
                potentialConvolutions.push_back(*it);
            }else{
                // do nothing
            }
        }

        // vykresli pročištěné kontury
        for( size_t i = 0; i< potentialConvolutions.size(); i++ ) {
            cv::Scalar color = CONTOUR_COLOR;
            cv::drawContours( originalImage, potentialConvolutions, i, color, 2, 8, hierarchy, 0, cv::Point() );
        }

        // odešli obrázky do GUI
        cv::Mat imageToShowOld, imageToShowNew;

        imageToShowOld = originalImage;                 //is BGR
        cv::cvtColor(image, imageToShowNew, CV_GRAY2BGR); //not BGR

        cvMatToQImage(&imageToShowOld, 1);
        cvMatToQImage(&imageToShowNew, 2);

        name = names->getNextImageRelativePath();
        counter++;

        image.release();
        preprocessedImage.release();
        originalImage.release();
    }
    emit imagesProcessed(counter);
}

/**
 * Metoda provede kontrolu zadaných kritérií.
 * Pokud kontura nevyhovuje toleranci je vráceno false a bude vyřazena.
 * @brief Cleaner::checkContour
 * @param contour
 * @return
 */
bool Cleaner::checkContour(std::vector<cv::Point> contour){
    std::vector<cv::Point> contourPoly;

    cv::approxPolyDP( cv::Mat(contour), contourPoly, 3, true );
    cv::Rect boundingRect = cv::boundingRect(cv::Mat(contourPoly));

    double area = cv::contourArea(contourPoly);
    double rectArea = boundingRect.width * boundingRect.height;

    bool result = true;
    if( boolMinVerticies ){
        result &= checkMinVerticies(contourPoly.size());
    }
    if( boolMaxVerticies ){
        result &= checkMaxVerticies(contourPoly.size());
    }
    if( boolAspectRatio){
        result &= checkAspectRatio(boundingRect.width, boundingRect.height);
    }
    if( boolExtent){
        result &= checkExtent(area, rectArea);
    }
    return result;
}

/**
 * Zkontroluje konturu na pravoúhlost kontury proti osanému rovnoběžníku
 * @brief Cleaner::checkExtent
 * @param contourArea
 * @param rectArea
 * @return
 */
bool Cleaner::checkExtent(double contourArea, double rectArea){
    double extent = double(contourArea)/rectArea;

    double epsilonExtent = descriptor->getEpsilonRatio();
    double reqExtent = descriptor->getReqAspectRatio();

    if(extent > (reqExtent + epsilonExtent) || extent  < (reqExtent - epsilonExtent)){
        return false;
    }
    return true;
}

/**
 * Zkontrolu je konturu na poměr stran opsaného rovnoběžníku
 * @brief Cleaner::checkAspectRatio
 * @param width
 * @param height
 * @return
 */
bool Cleaner::checkAspectRatio(int width, int height){
    double  aspectRatio = double(width)/height;

    double epsilonRatio = descriptor->getEpsilonRatio();
    double reqAspectRatio = descriptor->getReqAspectRatio();

    if(aspectRatio > (reqAspectRatio + epsilonRatio) || aspectRatio  < (reqAspectRatio - epsilonRatio)){
        return false;
    }
    return true;
}

/**
 * Zkontroluje konturu na maximální počet vrcholů
 * @brief Cleaner::checkMaxVerticies
 * @param verticies
 * @return
 */
bool Cleaner::checkMaxVerticies(unsigned int verticies){
    unsigned int reqMaxVerticies = descriptor->getReqMaxVerticies();
    if ( verticies > reqMaxVerticies ){
        return false;
    }
    return true;
}

/**
 * Zkontroluje konturu dle na minimální počet vrcholů
 * @brief Cleaner::checkMinVerticies
 * @param verticies
 * @return
 */
bool Cleaner::checkMinVerticies(unsigned int verticies){
    unsigned int reqMinVerticies = descriptor->getReqMinVerticies();
    if ( verticies < reqMinVerticies ){
        return false;
    }
    return true;
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void Cleaner::cannyEdges(cv::Mat src, cv::Mat out){
    int lowThreshold = 100;
    int ratio = 2;
    int kernelSize = 3;

    GaussianBlur(src, out, cv::Size(5,5), 0 );
    Canny( src, out, lowThreshold, lowThreshold*ratio, kernelSize);
}

/**
 * Převod cv:Mat image to QImage pro zobrazení v aplikaci
 * @brief Cleaner::cvMatToQImage
 * @param input
 * @param destination
 */
void Cleaner::cvMatToQImage(cv::Mat *input, int destination) {

    QImage *dest = new QImage((const uchar *) input->data, input->cols, input->rows, QImage::Format_RGB888);
    dest->bits(); // enforce deep copy, see documentation

    emit showImage(dest, destination); // draw prepared Image in window
}
