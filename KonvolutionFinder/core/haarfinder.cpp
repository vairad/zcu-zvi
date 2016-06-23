#include "haarfinder.h"

/**
 * Konstruktor připraví cascade clasifier
 * @brief HaarFinder::HaarFinder
 * @param names
 * @param inclusionCascadeName
 */
HaarFinder::HaarFinder(FilenameFactory *names, std::string inclusionCascadeName): names(names) {
    cascade_name = inclusionCascadeName;
    if( !cascade.load(cascade_name) ){
        throw FileNotAcceptableException(cascade_name.c_str());
    }
}

/**
 * Pracovní smyčka tohoto vlákna.
 * @brief HaarFinder::run
 */
void HaarFinder::run(){
    int counter = 0;
    cv::Mat image;
    cv::Mat originalImage;

    QString name = names->getNextImageRelativePath(); // nacti path prvniho obrazku

    while(name != NULL){
        //todo semaphore lock

        if(counter % 50 == 0 ){
            emit imagesProcessed(counter); // odešli průběžný počet zpracovaných obrázků do gui
        }

        originalImage = cv::imread(name.toStdString(), CV_LOAD_IMAGE_UNCHANGED); // nacti obrazek
        cv::cvtColor(originalImage, image, CV_BGR2GRAY);                        // priprav zpracovany obrazek

        if(!image.data){  // kontrola nacteni obrazku
            throw EmptyImageException(name.toStdString().c_str());
        }

        HistogramModifier modifier(255, 0);
        modifier.stretchHistogram(&image, &image, 0, 150);      // roztažení a přebarvení obrázku

        std::vector< cv::Rect > inclusions;
        inclusions = detectInclusions(image);               //hledani inkluzi

        drawInclusions( &inclusions, &image);               //vykresleni inkluzi


        // send images to GUI
        cv::Mat imageToShowOld, imageToShowNew;

        imageToShowOld = originalImage; // is BGR

        cv::cvtColor(image, imageToShowNew, CV_GRAY2BGR); // not BGR

        cvMatToQImage(&imageToShowOld, 1);
        cvMatToQImage(&imageToShowNew, 2);

        name = names->getNextImageRelativePath(); //read next image path
        counter++;

        // free images
        image.release();
        originalImage.release();
    }

    emit imagesProcessed(counter); // odešli závěrečný počet zpracovaných obrázků do gui
}

/**
 * Metoda vykresli kruhy kolem nalezenych inkluzí
 * @brief drawInclusions
 * @param inclusions vector bounding rects nalezených objektů
 * @param image obrázek pro zakreslení kruhů pro zvýraznění
 */
void HaarFinder::drawInclusions(std::vector< cv::Rect > *inclusions, cv::Mat *image){
    cv::Scalar color(0, 0, 0);
    for( size_t i = 0; i < inclusions->size(); i++ ) {
        cv::Point center( inclusions->at(i).x + inclusions->at(i).width*0.5,
                          inclusions->at(i).y + inclusions->at(i).height*0.5 );
        cv::ellipse( *image, center, cv::Size( inclusions->at(i).width*0.5, inclusions->at(i).height*0.5),
                            0, 0, 360, //angleRotation, startAngle, endAngle
                                color, 4); //color, thickness
    }
}

/**
 * Funkce snastavenými parametry pro vyhledávání inkluzí s využítím CascadeClasificator
 * @brief HaarFinder::detectInclusions
 * @param frame - BW image
 * @return vector of boundingRects
 */
std::vector< cv::Rect > HaarFinder::detectInclusions( cv::Mat frame ){
  std::vector< cv::Rect > inclusions;

  double scaleFactor = 1.2; // – Parameter specifying how much the image size is reduced at each image scale.
  int minNeighbors = 2; // – Parameter specifying how many neighbors each candidate rectangle should have to retain it.
  cv::Size minSize(50, 50); // – Minimum possible object size. Objects smaller than that are ignored.
  cv::Size maxSize(120, 120); // – Maximum possible object size. Objects larger than that are ignored.

  //– Detect inclusions
  cascade.detectMultiScale(frame, inclusions, scaleFactor, minNeighbors,
                                                    0|CV_HAAR_SCALE_IMAGE, minSize , maxSize);

  return inclusions;
 }

 //todo Vytvořit třídu QConverter
/**
 * Převod cv:Mat image to QImage pro zobrazení v aplikaci
 * @brief Cleaner::cvMatToQImage
 * @param input
 * @param destination
 */
void HaarFinder::cvMatToQImage(cv::Mat *input, int destination) {

    QImage *dest = new QImage((const uchar *) input->data, input->cols, input->rows, QImage::Format_RGB888);
    dest->bits(); // enforce deep copy, see documentation

    emit showImage(dest, destination); // draw prepared Image in window
}
