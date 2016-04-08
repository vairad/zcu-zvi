#include "core/cleaner.h"

#include <iostream>

#include "core/exception.h"
#include "core/histogrammodifier.h"

void Cleaner::setFactory(FilenameFactory *names){
    this->names = names;
    this->counter = 0;
    this->lowThreshold = 1;
    this->ratio = 1;
    this->kernelSize = 3;
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

//        std::cout << "Read: " << name.toStdString() << std::endl;
        cv::Mat image = cv::imread(name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

//         std::cout << "Image readed." << std::endl;
        if(!image.data){  // kontrola dat
            throw EmptyImageException(name.toStdString().c_str());
        }


        cv::Mat preprocessedImage = cv::Mat::zeros(image.rows, image.cols, image.type());;
//         std::cout << "Zero Mat prepared" << std::endl;

        HistogramModifier c(255, 0);
        c.setAlpha(1.0);
        c.setBeta(100);
        c.moveHistogram(&image, &preprocessedImage);

//        imwrite( "../data/moved.tif", preprocessedImage);
//        std::cout << "Histogram moved" << std::endl;

        c.stretchHistogram(&preprocessedImage, &preprocessedImage, 150, 255);


  //       imwrite( "../data/stretched3.tif", preprocessedImage);
//        std::cout << "Histogram stretched" << std::endl;

//        cv::adaptiveThreshold(preprocessedImage,preprocessedImage,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,27,2);

  //      cv::Sobel(preprocessedImage, preprocessedImage, preprocessedImage.depth(), 1, 1, 5, 2);

        //cv::threshold(preprocessedImage,preprocessedImage, 110, 255, CV_THRESH_BINARY);



        //morphologyClose(preprocessedImage, preprocessedImage);
        cannyEdges(preprocessedImage, preprocessedImage);



        std::vector< std::vector<cv::Point> > hulls;
        //convexHull(preprocessedImage, hulls);

        imwrite( "../data/processed.tif", preprocessedImage);

        cv::Mat imageToShowOld, imageToShowNew;
        cv::cvtColor(image, imageToShowOld, CV_GRAY2BGR);
        cv::cvtColor(preprocessedImage, imageToShowNew, CV_GRAY2BGR);
        cvMatToQImage(&imageToShowOld, 1);
        cvMatToQImage(&imageToShowNew, 2);
        name = names->getNextImageRelativePath();

        image.release();
        preprocessedImage.release();
    }
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void Cleaner::cannyEdges(cv::Mat src, cv::Mat out){
    GaussianBlur(src, out, cv::Size(5,5), 0 );
    Canny( src, out, lowThreshold, lowThreshold*ratio, kernelSize);
}

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
void Cleaner::convexHull(cv::Mat input,
                            std::vector< std::vector<cv::Point> > &out)
{
  std::vector< std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  // find contours
  cv::findContours( input, contours, hierarchy,
                      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,
                      cv::Point(0, 0) );

  // create convex hull
  std::vector< std::vector< cv::Point > > hull ( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
  {
    cv::convexHull( cv::Mat(contours[i]), hull[i], false );
  }

  out = hull;
}

void Cleaner::cvMatToQImage(cv::Mat *input, int destination) {

    QImage *dest = new QImage((const uchar *) input->data, input->cols, input->rows, QImage::Format_RGB888);
    dest->bits(); // enforce deep copy, see documentation

    emit showImage(dest, destination); // draw prepared Image in window
}
