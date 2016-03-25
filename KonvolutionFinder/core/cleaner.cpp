#include "core/cleaner.h"
#include <iostream>

#include "core/exception.h"

Cleaner::Cleaner(FilenameFactory *names)
{
    int counter = 0;
    QString name = names->getNextImagePath();
    while(name != NULL){
        counter++;
        cv::Mat average_image;
        cv::Mat double_image;
        cv::Mat image = cv::imread(name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
        image.convertTo(double_image, CV_64FC1, 1/255.0);

        if(!image.data){  // kontrola dat
            throw EmptyImageException(name.toStdString().c_str());
        }

        average_image = double_image;

        for (int i = 0; i < 10; ++i) {
//            std::cout << "Nacten obrazek " << i << "\n";
            names->getNextImagePath();
            cv::Mat image = cv::imread(name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
            image.convertTo(double_image, CV_64FC1, 1/255.0);
            cv::add(average_image, double_image, average_image);
        }

       std::cout << "Out " << counter <<"\n";
       std::cout.flush();

      // average_image = average_image * (1.0 / 10.0 );




        cv::namedWindow( "Average", CV_WINDOW_KEEPRATIO);// Create a window for display.
        cv::imshow( "Average", average_image );                   // Show our image inside it.

        cv::namedWindow( "Desaty", CV_WINDOW_KEEPRATIO);// Create a window for display.
        cv::imshow( "Desaty", image );                   // Show our image inside it.

        cv::waitKey(1);

        name = names->getNextImagePath();
    }

}
