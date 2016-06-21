#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QSlider>
#include <QLabel>


#include "core/filename_factory.h"
#include "core/convolutiondescriptor.h"

#include "gui/descriptordialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QImage *lastImageOriginal = NULL;
    QImage *lastImageProcessed = NULL;

    DescriptionDialog *description_dialog = NULL;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    FilenameFactory *filename_factory = NULL;
    ConvolutionDescriptor *convolution_descriptor = NULL;

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QWidget *sliderBar;
    QLabel *thresholdSliderLabel;
    QLabel *ratioSliderLabel;
    QLabel *kernelSliderLabel;
    QSlider *sliderThreshold;
    QSlider *sliderKernel;
    QSlider *sliderRatio;

    QString threshName = "Hodnota prahu: ";

    void createMenuBar();
    void createSliderBar();
    void createSliders();

    //void MainWindow::closeEvent(QCloseEvent *event);

    void writeOriginalImage(QImage *image);
    void writeNewImage(QImage *image);

    QMenu *createMenuFile(QMenuBar *menuBar);
    QMenu *createMenuAnalyze(QMenuBar *menuBar);
    QMenu *createMenuHelp(QMenuBar *menuBar);
private slots:
    void openFileChooser();
    void writeImage(QImage *image, int destination);
    void setThresholdLabelValue(int value);
    void setKernelLabelValue(int value);
    void setRatioLabelValue(int value);
    void startAnalyze();
    void saveXmlConvolution();
    void loadXmlConvolution();
    void newXmlConvolution();
    void showSetUp();
};

#endif // MAINWINDOW_H
