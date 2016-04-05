#include "core/cleaner.h"

#include <iostream>

#include "core/exception.h"
#include "core/histogrammodifier.h"

void Cleaner::setFactory(FilenameFactory *names){
    this->names = names;
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

//        imwrite( "../data/stretched.tif", preprocessedImage);
//        std::cout << "Histogram stretched" << std::endl;

//        cv::adaptiveThreshold(preprocessedImage,preprocessedImage,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,27,2);
        cv::threshold(preprocessedImage,preprocessedImage, 170, 255, CV_THRESH_BINARY);

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


void Cleaner::cvMatToQImage(cv::Mat *input, int destination) {

    QImage *dest = new QImage((const uchar *) input->data, input->cols, input->rows, QImage::Format_RGB888);
    dest->bits(); // enforce deep copy, see documentation

    emit showImage(dest, destination); // draw prepared Image in window
}
