#include "core/cleaner.h"

#include <iostream>

#include "core/exception.h"
#include "core/histogrammodifier.h"


int Cleaner::defaultThreshold = 160;

Cleaner::Cleaner(FilenameFactory *names, ConvolutionDescriptor *descriptor):  names(names), descriptor(descriptor){
    this->counter = 0;
    this->lowThreshold = 100;
    this->threshold = defaultThreshold;
    this->ratio = 2;
    this->kernelSize = 3;
}

void Cleaner::setThresh(int value){
    this->threshold = value;
}

void Cleaner::setLowThresh(int value){
    this->lowThreshold = value;
}

void Cleaner::setRatio(int value){
    this->ratio = value;
}

void Cleaner::setKernelSize(int value){
    this->kernelSize = value;
}

void Cleaner::run(){
    QString name = names->getNextImageRelativePath();
    while(name != NULL){
        counter++;
        if(counter % 100 == 0 ){
            std::cout << counter << std::endl;
        }

        cv::Mat originalImage = cv::imread(name.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
        cv::Mat image;
        cv::cvtColor(originalImage, image, CV_BGR2GRAY);

        if(!image.data){  // kontrola dat
            throw EmptyImageException(name.toStdString().c_str());
        }

        cv::Mat preprocessedImage = cv::Mat::zeros(image.rows, image.cols, image.type());

        HistogramModifier modifier(255, 0);
        modifier.stretchHistogram(&image, &image, 0, 150);

        cv::threshold(image, image, threshold, 255, CV_THRESH_BINARY);

        cannyEdges(image, preprocessedImage);

        std::vector< std::vector<cv::Point> > hulls; //obálky objektů.
        std::vector< std::vector<cv::Point> > potentialConvolutions; //obálky objektů splňující podmínky.
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours( preprocessedImage, hulls, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,
                            cv::Point(0, 0) );

        /// Draw contours
        for( size_t i = 0; i< hulls.size(); i++ ) {
            cv::Scalar color = CONTOUR_COLOR_II;
            cv::drawContours( originalImage, hulls, i, color, 2, 8, hierarchy, 0, cv::Point() );
        }
        /// filter contours
        for(std::vector< std::vector<cv::Point> >::iterator it = hulls.begin(); it != hulls.end(); ++it) {
            if(checkContour(*it)){
                potentialConvolutions.push_back(*it);
            }else{
                // do nothing
            }
        }

        /// Draw contours
        for( size_t i = 0; i< potentialConvolutions.size(); i++ ) {
            cv::Scalar color = CONTOUR_COLOR;
            cv::drawContours( originalImage, potentialConvolutions, i, color, 2, 8, hierarchy, 0, cv::Point() );
        }

   //    imwrite( name.replace(".tif", "modified.tif").toStdString(), originalImage);


        /// show image
        cv::Mat imageToShowOld, imageToShowNew;
        //cv::cvtColor(image, imageToShowOld, CV_GRAY2BGR);
        imageToShowOld = originalImage;
        cv::cvtColor(image, imageToShowNew, CV_GRAY2BGR);
        cvMatToQImage(&imageToShowOld, 1);
        cvMatToQImage(&imageToShowNew, 2);
        name = names->getNextImageRelativePath();

        image.release();
        preprocessedImage.release();
        originalImage.release();
    }
}

bool Cleaner::checkContour(std::vector<cv::Point> contour){
    std::vector<cv::Point> contourPoly;

    cv::approxPolyDP( cv::Mat(contour), contourPoly, 3, true );
    cv::Rect boundingRect = cv::boundingRect(cv::Mat(contourPoly));
    double  aspectRatio = double(boundingRect.width)/boundingRect.height;
    double contourLength = cv::arcLength(contourPoly, true);

    double reqAspectRatio = descriptor->getReqAspectRatio();
    double epsilonRatio = descriptor->getEpsilonRatio();

    int reqMaxVerticies = descriptor->getReqMaxVerticies();
    int reqMinVerticies = descriptor->getReqMinVerticies();

    if( contourPoly.size() < reqMinVerticies ){
        return false;
    }
    if( contourPoly.size() > reqMaxVerticies ){
        return false;
    }
    if(aspectRatio > (reqAspectRatio + epsilonRatio) || aspectRatio  < (reqAspectRatio - epsilonRatio) ){
        return false;
    }

    return true;
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void Cleaner::cannyEdges(cv::Mat src, cv::Mat out){
    GaussianBlur(src, out, cv::Size(5,5), 0 );
    Canny( src, out, lowThreshold, lowThreshold*ratio, kernelSize);
}


/**
 * Funkce použije pro opezaci uzavření objektů funkcionalitu dostupnou v knihovně openCV
 * @brief Cleaner::morphologyClose
 * @param src zdrojový cv::Mat
 * @param out výsledný cv::Mat
 */
void Cleaner::morphologyClose(cv::Mat src, cv::Mat out){
    int size = 3;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                  cv::Size(2*size+1, 2*size+1),
                                                  cv::Point(size, size) );

    cv::morphologyEx( src, out, CV_MOP_CLOSE, element);
}


/// This method finds convex hulls of all objects in the picture
///
/// @param input frame to find objects
/// @param out reference to vector of convex hulls
///
void Cleaner::convexHull(cv::Mat input, std::vector< std::vector<cv::Point> > &out)
{
  std::vector< std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  // find contours
  cv::findContours( input, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,
                      cv::Point(0, 0) );

  // create convex hull
  std::vector< std::vector<cv::Point> > hull ( contours.size() );

  for(unsigned int i = 0; i < contours.size(); i++ )
  {
    cv::convexHull( cv::Mat(contours[i]), hull[i], false );
  }

  out = hull;
}

void Cleaner::fillAllContours(cv::Mat img, const std::vector< std::vector<cv::Point> > &contours){
   for(size_t i = 0; i < contours.size(); i++){
        cv::drawContours(img, contours, i, cv::Scalar(0, 255, 255), CV_FILLED); // fill GREEN
   }
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
