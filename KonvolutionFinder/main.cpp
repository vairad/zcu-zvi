#include "mainwindow.h"
#include <QApplication>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

//#include "core/cleaner.h"
//#include "core/filename_factory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

//    FilenameFactory *name_factory = new FilenameFactory("../data/Z Serier S57H19/Zvetseni 5x/");

//    QString name;
//    name = name_factory->getNextImageName();
  /*  while(name != NULL){
        cout << name.toStdString() << "\n";
        name = name_factory->getNextImagePath();
    }*/

 //   Cleaner cleaner(w.filename_factory);

}
