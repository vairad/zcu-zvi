#include "mainwindow.h"
#include <QApplication>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "core/cleaner.h"
#include "core/filename_factory.h"

using namespace cv;
using namespace std;

int kamera(){

    VideoCapture cap(0); // open the video file for reading

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

   // double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

   //  cout << "Frame per seconds : " << fps << endl;

   namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    int i = 0;
    while(i < 500)
    {
        Mat frame;

        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
                        cout << "Cannot read the frame from video file" << endl;
                       break;
        }

        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        int key = waitKey(30) % 256; //cause bug in opencv
        cout << key << endl;

        if(key == 'q') //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl;
                break;
       }
        ++i;
    }

    return 0;
}

int main(int argc, char *argv[])
{ /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/

//    kamera();

    FilenameFactory *name_factory = new FilenameFactory("../data/Z Serier S57H19/Zvetseni 5x/");

//    QString name;
//    name = name_factory->getNextImageName();
  /*  while(name != NULL){
        cout << name.toStdString() << "\n";
        name = name_factory->getNextImagePath();
    }*/

    Cleaner cleaner(name_factory);

}
