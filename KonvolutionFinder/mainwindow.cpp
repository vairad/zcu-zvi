#include <QApplication>
#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/cleaner.h"
#include "test/testclass.h"
#include "core/exception.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Konvolution Finder");
    this->showMaximized();

    createMenuBar();

    convolution_descriptor = new ConvolutionDescriptor();
    centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    createSliderBar();
    layout->addWidget(sliderBar);
    centralWidget->setLayout(layout);
  /*  this->setCentralWidget(centralWidget);*/

    createSliderBar();
}

/**
 * @brief MainWindow::createMenuFile
 * @return
 */
QMenu *MainWindow::createMenuFile(QMenuBar *menuBar){
    // polozka Soubor
    QMenu *menuFile;
    menuFile = new QMenu(menuBar);
    menuFile->setTitle("Soubor");

    // akce nacist slozku
    QAction *actionOpen;
    actionOpen = new QAction(this);
    actionOpen->setText("Načíst složku...");
    actionOpen->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFileChooser()));
    menuFile->addAction(actionOpen);

    // akce ukoncit aplikaci
    QAction *actionQuit;
    actionQuit = new QAction(this);
    actionQuit->setText("Ukončit aplikaci");
    actionQuit->setShortcut(Qt::Key_Q | Qt::CTRL);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    menuFile->addAction(actionQuit);

    return menuFile;
}

/**
 * @brief MainWindow::createMenuAnalyze
 * @return
 */
QMenu *MainWindow::createMenuAnalyze(QMenuBar *menuBar){
    //    polozka Analyza
    QMenu *menuAnalyze;
    menuAnalyze = new QMenu(menuBar);
    menuAnalyze->setTitle("Analýza");


    // akce spust analyzu
    QAction *actionStartAnalyze;
    actionStartAnalyze = new QAction(this);
    actionStartAnalyze->setText("Spusť analýzu");
    actionStartAnalyze->setShortcut(Qt::Key_A | Qt::CTRL);
    connect(actionStartAnalyze, SIGNAL(triggered()), this, SLOT(startAnalyze()));
    menuAnalyze->addAction(actionStartAnalyze);

    // akce nastavení
    QAction *actionOpenSetup;
    actionOpenSetup = new QAction(this);
    actionOpenSetup->setText(tr("Nastavení"));
//    actionOpenSetup->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(actionOpenSetup, SIGNAL(triggered()), this, SLOT(showSetUp()));
    menuAnalyze->addAction(actionOpenSetup);

    // akce nacti popis konvoluce
    QAction *actionLoadDescriprion;
    actionLoadDescriprion = new QAction(this);
    actionLoadDescriprion->setText("Načti popis konvoluce");
    actionLoadDescriprion->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(actionLoadDescriprion, SIGNAL(triggered()), this, SLOT(loadXmlConvolution()));
    menuAnalyze->addAction(actionLoadDescriprion);

    // akce uloz popis konvoluce
    QAction *actionSaveDescriprion;
    actionSaveDescriprion = new QAction(this);
    actionSaveDescriprion->setText("Ulož popis konvoluce");
    actionSaveDescriprion->setShortcut(Qt::Key_S | Qt::CTRL);
    connect(actionSaveDescriprion, SIGNAL(triggered()), this, SLOT(saveXmlConvolution()));
    menuAnalyze->addAction(actionSaveDescriprion);

    return menuAnalyze;
}

/**
 * @brief MainWindow::createMenuHelp
 * @return
 */
QMenu *MainWindow::createMenuHelp(QMenuBar *menuBar){
    // polozka Napoveda
    QMenu *menuHelp;
    menuHelp = new QMenu(menuBar);
    menuHelp->setTitle("Nápověda");


    /*   QAction *actionHelp;
       actionHelp = new QAction(this);
       actionHelp->setText("Nápověda k aplikaci");
       actionHelp->setShortcut(Qt::Key_F1);
       connect(actionHelp, SIGNAL(triggered()), this, SLOT(openHelp()));
       menuHelp->addAction(actionHelp);

       QAction *actionAboutApp;
       actionAboutApp = new QAction(this);
       actionAboutApp->setText("O aplikaci");
       connect(actionAboutApp, SIGNAL(triggered()), this, SLOT(aboutApplication()));
       menuHelp->addAction(actionAboutApp);*/

    return menuHelp;
}



/**
 * Vytvori menu aplikace
 * @brief MainWindow::createMenuBar
 */
void MainWindow::createMenuBar() {
    QMenuBar *menuBar;
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 756, 21));

    menuBar->addAction(createMenuFile(menuBar)->menuAction());
    menuBar->addAction(createMenuAnalyze(menuBar)->menuAction());
    menuBar->addAction(createMenuHelp(menuBar)->menuAction());

    this->setMenuBar(menuBar);
}

void MainWindow::createSliderBar() {
    sliderBar = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(sliderBar);

    sliderThreshold = new QSlider();
    sliderThreshold->setOrientation(Qt::Horizontal);
    sliderThreshold->setRange(0, 255);
    sliderThreshold->setValue(Cleaner::defaultThreshold);
    thresholdSliderLabel = new QLabel(threshName + QString::number(Cleaner::defaultThreshold));
    connect(sliderThreshold, SIGNAL(valueChanged(int)), this, SLOT(setThresholdLabelValue(int)));

    sliderRatio = new QSlider();
    sliderRatio->setOrientation(Qt::Horizontal);
    sliderRatio->setRange(1, 10);
    sliderRatio->setValue(1);
    ratioSliderLabel = new QLabel("Ratio: 1");
    connect(sliderRatio, SIGNAL(valueChanged(int)), this, SLOT(setRatioLabelValue(int)));
    sliderRatio->setDisabled(true);

    sliderKernel = new QSlider();
    sliderKernel->setOrientation(Qt::Horizontal);
    sliderKernel->setRange(3, 50);
    sliderKernel->setValue(3);
    kernelSliderLabel = new QLabel("Kernel Size: 3");
    connect(sliderKernel, SIGNAL(valueChanged(int)), this, SLOT(setKernelLabelValue(int)));
    sliderKernel->setDisabled(true);

    QWidget *threshWidget = new QWidget();
    QVBoxLayout *threshLayout = new QVBoxLayout(threshWidget);
    threshLayout->addWidget(thresholdSliderLabel);
    threshLayout->addWidget(sliderThreshold);

    QWidget *ratioWidget = new QWidget();
    QVBoxLayout *ratioLayout = new QVBoxLayout(ratioWidget);
    ratioLayout->addWidget(ratioSliderLabel);
    ratioLayout->addWidget(sliderRatio);

    QWidget *kernelWidget = new QWidget();
    QVBoxLayout *kernelLayout = new QVBoxLayout(kernelWidget);
    kernelLayout->addWidget(kernelSliderLabel);
    kernelLayout->addWidget(sliderKernel);

    layout->addWidget(threshWidget);
    layout->addWidget(ratioWidget);
    layout->addWidget(kernelWidget);

    this->ui->verticalLayout->addWidget(sliderBar);
}

void MainWindow::setThresholdLabelValue(int value) {
    thresholdSliderLabel->setText(threshName + QString::number(value));
}
void MainWindow::setRatioLabelValue(int value) {
    ratioSliderLabel->setText("Ratio: " + QString::number(value));
}
void MainWindow::setKernelLabelValue(int value) {
    kernelSliderLabel->setText("Kernel Size: " + QString::number(value));
}


void MainWindow::startAnalyze(){
    if(filename_factory == NULL){
          filename_factory = new FilenameFactory("../data/test");
    }
    //    TestConsole::testFilenameFactory(filename_factory);
        try{
            Cleaner *cleaner = new Cleaner(filename_factory, convolution_descriptor);

            connect(cleaner, SIGNAL(showImage(QImage *, int)), this, SLOT(writeImage(QImage *, int)));
            connect(sliderThreshold, SIGNAL(valueChanged(int)), cleaner, SLOT(setThresh(int)));
            connect(sliderRatio, SIGNAL(valueChanged(int)), cleaner, SLOT(setKernelSize(int)));
            connect(sliderKernel, SIGNAL(valueChanged(int)), cleaner, SLOT(setRatio(int)));

            cleaner->start();

        }catch(EmptyImageException &e){
            std::cout << e.what() << std::endl;
        }catch(std::exception &e){
            std::cout << "unnamed exception" << "\n";
            std::cout << e.what() << "\n";
        }
}


/**
 * Otevre dialog pro vyber souboru
 * @brief MainWindow::openFileChooser
 * @return jmeno vybrane slozky
 */
void MainWindow::openFileChooser() {
    QString folder = QFileDialog::getExistingDirectory(this, tr("Vybrat složku"),
                     QDir::currentPath(), QFileDialog::DontResolveSymlinks);

    if (!QString::compare(folder, "")) {
        return;
    }

    filename_factory = new FilenameFactory(folder);
}

/**
 * Dotaz pred zavrenim aplikace
 * @brief MainWindow::closeEvent
 * @param event
 */
/*void MainWindow::closeEvent(QCloseEvent *event) {
    int reply = QMessageBox::question(this, "Ukončit aplikaci", "Opravdu chcete ukončit aplikaci?", "Ano", "Ne");
    if (reply == 0) {
        QApplication::closeAllWindows();
        event->accept();
    } else {
        event->ignore();
    }
}*/

/**
 * Zpracuj signál přinášející obrázek k vykreslení
 * @brief MainWindow::writeImage
 * @param image pointer na obrázek
 * @param destination id umístění
 */
void MainWindow::writeImage(QImage *image, int destination){
    switch(destination){
        case 1:
            writeOriginalImage(image); break;
        case 2:
            writeNewImage(image); break;
        default:
            std::cout << "Uknown option writeImage()" << std::endl;
    }
}


/**
 * Vykresli QImage do labelu imageOriginal (levy)
 * @brief MainWindow::writeNewImage
 * @param image pointer na QImage
 */
void MainWindow::writeOriginalImage(QImage *image){
    //zjisti rozmery labelu pro vykresleni
    int width = this->ui->imageOriginal->width();
    int height = this->ui->imageOriginal->height();

    if(lastImageOriginal != NULL){ //uvolni předchozí obrázek
        delete lastImageOriginal;
    }

    lastImageOriginal = image;

//    std::cout << "Kresli do original W: " << width << " H: " << height << "\n";

    //priprav obrazek spravnych rozmeru a barevne skladby (BGR -> RGB)
    QImage tmp = lastImageOriginal->rgbSwapped().scaled(width, height, Qt::KeepAspectRatio);
    //vykresli obrazek do labelu
    this->ui->imageOriginal->setPixmap(QPixmap::fromImage(tmp));
}


/**
 * Vykresli QImage do labelu imageProcessed (pravy)
 * @brief MainWindow::writeNewImage
 * @param image pointer na QImage
 */
void MainWindow::writeNewImage(QImage *image){
    //zjisti rozmery labelu pro vykresleni
    int width = this->ui->imageProcessed->width();
    int height = this->ui->imageProcessed->height();

    if(lastImageProcessed != NULL){ //uvolni předchozí obrázek
        delete lastImageProcessed;
    }

    lastImageProcessed = image;

//    std::cout << "Kresli do processed W: " << width << " H: " << height << "\n";

    //priprav obrazek spravnych rozmeru a barevne skladby (BGR -> RGB)
    QImage tmp = lastImageProcessed->rgbSwapped().scaled(width, height, Qt::KeepAspectRatio);
    //vykresli obrazek do labelu
    this->ui->imageProcessed->setPixmap(QPixmap::fromImage(tmp));
}

/**
 * Zprostředkuje uložení xml popisu vlastností konvoluce
 * @brief MainWindow::saveXmlConvolution
 */
void MainWindow::saveXmlConvolution(){
    if(convolution_descriptor == NULL){
        QMessageBox messageBox;
        messageBox.information(0,"","Není připravený popis k uložení.");
        messageBox.setFixedSize(500,200);
        return;
    }
    QString path;

    if(!QString::compare(convolution_descriptor->getFILE_NAME(), "")){
        path = QFileDialog::getSaveFileName(this, tr("Uložení popisu koncoluce"),
                         QDir::currentPath(), tr("XML Files (*.xml)"));
        convolution_descriptor->setFILE_NAME(path);
        description_dialog->mainTab->updatePath();
    }else{
        path = convolution_descriptor->getFILE_NAME();
    }

    try{
        convolution_descriptor->save(path);

        QMessageBox messageBox;
        messageBox.information(0,path,"Soubor: "+path+" byl uložen.");
        messageBox.setFixedSize(500,200);
    }catch(FileNotWriteableException &e){
        QMessageBox messageBox;
        messageBox.critical(0,path,"Do souboru: "+path+" nelze zapisovat");
        messageBox.setFixedSize(500,200);
    }
}


/**
 * Zprostředkuje načtení xml popisu vlastností konvoluce
 * @brief MainWindow::loadXmlConvolution
 */
void MainWindow::loadXmlConvolution(){
    QString folder = QFileDialog::getOpenFileName(this, tr("Open Convolution description"),
                                                  QDir::currentPath(), tr("XML Files (*.xml)"));

    if (!QString::compare(folder, "")) {
        QMessageBox messageBox;
        messageBox.information(0,"","Nebyl zvolen soubor");
        messageBox.setFixedSize(500,200);
        return;
    }

    // todo kontrola, zda nebezi analyza
    convolution_descriptor = new ConvolutionDescriptor(folder);

    if(!convolution_descriptor->isOk()){
        if (!QString::compare(folder, "")) {
            QMessageBox messageBox;
            messageBox.critical(0,"","Soubor není načtený");
            messageBox.setFixedSize(500,200);
        }
        delete convolution_descriptor;
        convolution_descriptor = NULL;
    }

    return;
}

/**
 * Zobrazí dialog nastavení analýzy.
 * @brief MainWindow::showSetUp
 */
void MainWindow::showSetUp(){
    if(description_dialog == NULL){
        description_dialog = new DescriptionDialog(convolution_descriptor);
    }
    description_dialog->show();
}

/**
 * Destruktor hlavního okna
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
