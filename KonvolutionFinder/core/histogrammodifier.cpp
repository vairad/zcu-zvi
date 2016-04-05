#include "histogrammodifier.h"

HistogramModifier::HistogramModifier(double high_bright, double low_bright){
    this->high_bright = high_bright;
    this->low_bright = low_bright;

    this->bright_size = high_bright - low_bright;

    alpha = 1.0;
    beta = 100;
}

//==PUBLIC METHODS=========================================================================
/**
 * Parametr ovlivňující posun intenzity obrazových bodů.
 * @brief HistogramModifier::setBeta
 * @param betaToSet hodnota mezi high_bright a -high_bright
 * @return
 */
bool HistogramModifier::setBeta(int betaToSet){
    if(betaToSet > -high_bright && betaToSet < high_bright){
        this->beta = betaToSet;
        return true;
    }else{
        return false;
    }
}


bool HistogramModifier::setAlpha(double alphaToSet){
    this->alpha = alphaToSet;
    return true;
}

//==MODIFIING METHODS======================================================================
/**
 * "Posune histogram o zadanou hodnotu beta"
 * Ke všem hodnotám snímku přičte  konstantu beta.
 * Hodnoty, které přetečou jsou přebarveny na černou
 *
 * @brief HistogramModifier::moveHistogram
 * @param imageToChange vstup
 * @param outputImage výstup
 */
void HistogramModifier::moveHistogram(cv::Mat *imageToChange, cv::Mat *outputImage) {
    // Do the operation new_image(i,j) = image(i,j) + beta
    for( int y = 0; y < imageToChange->rows; y++ ){
        for( int x = 0; x < imageToChange->cols; x++ ){
            int new_intensity = (alpha * imageToChange->at<unsigned char>(y,x)) + beta;
            //set value in range
            outputImage->at<unsigned char>(y,x) = roundBright(new_intensity);
        }
    }
}


/**
 * Roztáhne histogram do vnitřně nastavených mezí rozsahu z přesaných hodnot
 * @brief HistogramModifier::stretchHistogram
 * @param imageToChange vstupní obrázek
 * @param outputImage výstupní obrázek
 * @param low dolní hranice jasu k roztažení
 * @param high horní hranice jasu k roztažení
 */
void HistogramModifier::stretchHistogram(cv::Mat *imageToChange, cv::Mat *outputImage, double low, double high){

    double stretch_ratio = ((bright_size) / (high - low)) ;

    for( int y = 0; y < imageToChange->rows; y++ ){
        for( int x = 0; x < imageToChange->cols; x++ ){
            //compute new intensity
            int new_intensity = stretch_ratio * (imageToChange->at<unsigned char>(y,x) - low) + low_bright;
            //set value in range
            outputImage->at<unsigned char>(y,x) = roundBright(new_intensity);
        }
    }

}


//==PRIVATE METHODS========================================================================


/**
 * Zabrání přetečení hodnoty dlerozsahu low_bright a high_bright
 * @brief HistogramModifier::roundBright
 * @param value hodnota k úpravě
 * @return korektní rozsah jasu
 */
unsigned char HistogramModifier::roundBright(double value)
{
    if( value >= high_bright){
        return high_bright;
    }else if( value <= low_bright ){
        return low_bright;
    }else{
       return (unsigned char)value;
    }
}

//========================================================================================

HistogramModifier::~HistogramModifier(){

}
