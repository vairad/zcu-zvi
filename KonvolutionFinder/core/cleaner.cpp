#include "core/cleaner.h"

#include <iostream>

#include "core/exception.h"

void Cleaner::setFactory(FilenameFactory *names){
    this->names = names;
}

void Cleaner::run(){
    QString name = names->getNextImageRelativePath();
    while(name != NULL){

        std::cout << "Read: " << name.toStdString() <<"\n";
        std::cout.flush();

        cv::Mat image = cv::imread(name.toStdString(), CV_LOAD_IMAGE_COLOR);

        if(!image.data){  // kontrola dat
            throw EmptyImageException(name.toStdString().c_str());
        }

 //       cv::namedWindow( "Desaty", CV_WINDOW_KEEPRATIO);// Create a window for display.
 //       cv::imshow( "Desaty", image );                   // Show our image inside it.


        msleep(1000);
        cvMatToQImage(&image);

       // name = names->getNextImagePath();
        name = names->getNextImageRelativePath();
    }
}

void Cleaner::run2(){
    int counter = 0;
    QString name = names->getNextImagePath();
    while(name != NULL){

        std::cout << "Read: " << name.toStdString() <<"\n";
        std::cout.flush();

        counter++;
        cv::Mat average_image;
        cv::Mat double_image;
        cv::Mat image = cv::imread(name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
        image.convertTo(double_image, CV_64FC1, 1/255.0);

        if(!image.data){  // kontrola dat
            throw EmptyImageException(name.toStdString().c_str());
        }

        average_image = double_image;

 /*       for (int i = 0; i < 10; ++i) {
//            std::cout << "Nacten obrazek " << i << "\n";
            names->getNextImagePath();
            cv::Mat image = cv::imread(name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
            image.convertTo(double_image, CV_64FC1, 1/255.0);
            cv::add(average_image, double_image, average_image);
        }*/

       std::cout << "Out " << counter <<"\n";
       std::cout.flush();

      // average_image = average_image * (1.0 / 10.0 );




//        cv::namedWindow( "Average", CV_WINDOW_KEEPRATIO);// Create a window for display.
//        cv::imshow( "Average", average_image );                   // Show our image inside it.

        cv::namedWindow( "Desaty", CV_WINDOW_KEEPRATIO);// Create a window for display.
        cv::imshow( "Desaty", image );                   // Show our image inside it.

        cvMatToQImage(&double_image);

        name = names->getNextImagePath();
    }
}

void Cleaner::cvMatToQImage(cv::Mat *input) {

    QImage *dest = new QImage((const uchar *) input->data, input->cols, input->rows, QImage::Format_RGB888);
    dest->bits(); // enforce deep copy, see documentation

    emit showImage(dest); // draw prepared Image in window
}
