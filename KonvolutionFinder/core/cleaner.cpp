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


void Cleaner::cvMatToQImage(cv::Mat *input) {

    QImage *dest = new QImage((const uchar *) input->data, input->cols, input->rows, QImage::Format_RGB888);
    dest->bits(); // enforce deep copy, see documentation

    emit showImage(dest); // draw prepared Image in window
}
