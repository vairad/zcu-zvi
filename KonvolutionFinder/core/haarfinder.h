#ifndef HAARFINDER_H
#define HAARFINDER_H

#include <string>

#include <QThread>
#include <QImage>
#include <QObject>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "core/filename_factory.h"
#include "core/exception.h"
#include "core/histogrammodifier.h"


/** *************************************************************************************
 * Třída která k identifikaci inkluzí používá CascadeClassifier z knihoven opencv.
 * @brief The HaarFinder class
 * @version 1.0.0
 */
class HaarFinder : public QThread {
    Q_OBJECT
    FilenameFactory *names; // názvy souborů
    cv::CascadeClassifier cascade; // klasifikátor
    std::string cascade_name; // path ke klasifikátoru

    std::vector<cv::Rect> detectInclusions( cv::Mat frame ); // metoda detekující inkluze

    void drawInclusions(std::vector<cv::Rect> *inclusions, cv::Mat *image); // metoda vykreslujici vysledky do obrazku
public:
    HaarFinder(FilenameFactory *names, std::string inclusionCascadeName);

    virtual void run(); // pracovní smyčka vlákna
    void cvMatToQImage(cv::Mat *input, int destination); // snad bude přesunuta do statického konvertoru
signals:
    void showImage(QImage *image, int destination); //signál do GUI obsahující obrázek k zobrazení
    void imagesProcessed(unsigned int count); // signál do GUI obsahující počet zpracovaných snímků
};

#endif // HAARFINDER_H
