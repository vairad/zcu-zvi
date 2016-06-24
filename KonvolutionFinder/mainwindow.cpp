#include <QApplication>
#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QMenuBar>
#include <QMessageBox>
#include <QAction>

#include "gui/helpwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/cleaner.h"
#include "test/testclass.h"
#include "core/exception.h"
#include "core/haarfinder.h"

#include <iostream>

/** **********************************************************************************
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    APP_NAME = new QString("InclusionFinder");
    ui->setupUi(this);
    this->setWindowTitle(*APP_NAME);
    this->showMaximized();

    createMenuBar();
    createToolBar();
    createToolBarSwitch();
    createToolBarCount();

    convolution_descriptor = new ConvolutionDescriptor();
    description_dialog = new DescriptionDialog(convolution_descriptor);
    centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    createSliderBar();
    layout->addWidget(sliderBar);
    centralWidget->setLayout(layout);
  /*  this->setCentralWidget(centralWidget);*/

    createSliderBar();
}

/**
 * Vytvori listu nastroju
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBar() {
    QToolBar *toolbar = addToolBar("main toolbar");

    QWidget* spacer = new QWidget();
    spacer->setFixedWidth(5);
    toolbar->addWidget(spacer);

    // akce pro zahajeni snimani
    QAction *startA = new QAction(this);
    startA->setObjectName(QStringLiteral("action"));
    startA->setText("Spustit analýzu");
    QPixmap start("images/start.png");
    startA->setIcon(QIcon(start));
    connect(startA, SIGNAL(triggered()), this, SLOT(startAnalyze())); //TODO
    toolbar->addAction(startA);


    // akce pro ukonceni snimani
    QAction *stopA = new QAction(this);
    stopA->setObjectName(QStringLiteral("action"));
    stopA->setText("Zastavit analýzu");
    QPixmap stop("images/stop.png");
    stopA->setIcon(QIcon(stop));
  //TODO  connect(stopA, SIGNAL(triggered()), this, SLOT(stopScanning()));
    toolbar->addAction(stopA);

    QWidget* spacer2 = new QWidget();
    spacer2->setFixedWidth(10);
    toolbar->addWidget(spacer2);

    toolbar->addSeparator();
}

/**
 * Vytvori listu nastroju
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBarSwitch() {
    QToolBar *toolbar = addToolBar("switch bar");

    QWidget* spacer = new QWidget();
    spacer->setFixedWidth(5);

    sliderSwitch = new QSlider();
    sliderSwitch->setOrientation(Qt::Horizontal);
    sliderSwitch->setRange(0, 1);
    sliderSwitch->setValue(this->clasificator);
    sliderSwitch->setFixedWidth(50);

    QLabel *contourLabel = new QLabel(tr("Vlastonsti kontur"));
    QLabel *classificatorLabel = new QLabel(tr("Kaskádový klasifikátor"));

    connect(sliderSwitch, SIGNAL(valueChanged(int)), this, SLOT(changeClasificator(int)));

    QWidget* spacer2 = new QWidget();
    spacer2->setFixedWidth(10);

    QWidget* spacer3 = new QWidget();
    spacer3->setFixedWidth(10);

    toolbar->addWidget(spacer);
    toolbar->addWidget(contourLabel);
    toolbar->addWidget(spacer2);
    toolbar->addWidget(sliderSwitch);
    toolbar->addWidget(spacer3);
    toolbar->addWidget(classificatorLabel);

    toolbar->addSeparator();
}

/**
 * Vytvori listu nastroju
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBarCount() {
    QToolBar *toolbar = addToolBar("count bar");

    QWidget* spacer = new QWidget();
    spacer->setFixedWidth(5);
    QWidget* spacer2 = new QWidget();
    spacer2->setFixedWidth(10);

    QLabel *countLabel = new QLabel(tr("Počet zpracovaných snímků:"));
    countL = new QLabel(tr("0"));


    toolbar->addWidget(spacer);
    toolbar->addWidget(countLabel);
    toolbar->addWidget(spacer2);
    toolbar->addWidget(countL);
    toolbar->addSeparator();
}

/** **********************************************************************************
 * @brief MainWindow::createMenuFile
 *
 * Metoda připraví svislé menu obsahující ovládání zdrojových dat.
 *
 * @return připravené menu
 */
QMenu *MainWindow::createMenuFile(QMenuBar *menuBar){
    // polozka Soubor
    QMenu *menuFile;
    menuFile = new QMenu(menuBar);
    menuFile->setTitle(tr("Soubor"));

    // akce nacist slozku
    QAction *actionOpen;
    actionOpen = new QAction(this);
    actionOpen->setText(tr("Načíst složku..."));
    actionOpen->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFileChooser()));
    menuFile->addAction(actionOpen);

    // akce ukoncit aplikaci
    QAction *actionQuit;
    actionQuit = new QAction(this);
    actionQuit->setText(tr("Ukončit aplikaci"));
    actionQuit->setShortcut(Qt::Key_Q | Qt::CTRL);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    menuFile->addAction(actionQuit);

    return menuFile;
}

/** **********************************************************************************
 * @brief MainWindow::createMenuAnalyze
 * Metoda připraví menu s ovládacími prvky nastavení analýzy
 * @return menu analyze
 */
QMenu *MainWindow::createMenuAnalyze(QMenuBar *menuBar){
    //    polozka Analyza
    QMenu *menuAnalyze;
    menuAnalyze = new QMenu(menuBar);
    menuAnalyze->setTitle(tr("Analýza"));


    // akce spust analyzu
    QAction *actionStartAnalyze;
    actionStartAnalyze = new QAction(this);
    actionStartAnalyze->setText(tr("Spusť analýzu"));
    actionStartAnalyze->setShortcut(Qt::Key_A | Qt::CTRL);
    connect(actionStartAnalyze, SIGNAL(triggered()), this, SLOT(startAnalyze()));
    menuAnalyze->addAction(actionStartAnalyze);

    menuAnalyze->addSeparator();

    // akce nastavení
    QAction *actionOpenSetup;
    actionOpenSetup = new QAction(this);
    actionOpenSetup->setText(tr("Nastavení"));
//    actionOpenSetup->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(actionOpenSetup, SIGNAL(triggered()), this, SLOT(showSetUp()));
    menuAnalyze->addAction(actionOpenSetup);

    menuAnalyze->addSeparator();

    // akce nacti popis konvoluce
    QAction *actionLoadDescriprion;
    actionLoadDescriprion = new QAction(this);
    actionLoadDescriprion->setText(tr("Načti popis inkluze"));
    actionLoadDescriprion->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(actionLoadDescriprion, SIGNAL(triggered()), this, SLOT(loadXmlConvolution()));
    menuAnalyze->addAction(actionLoadDescriprion);

    // akce uloz popis konvoluce
    QAction *actionSaveDescriprion;
    actionSaveDescriprion = new QAction(this);
    actionSaveDescriprion->setText(tr("Ulož popis inkluze"));
    actionSaveDescriprion->setShortcut(Qt::Key_S | Qt::CTRL);
    connect(actionSaveDescriprion, SIGNAL(triggered()), this, SLOT(saveXmlConvolution()));
    menuAnalyze->addAction(actionSaveDescriprion);

    // akce novy popis konvoluce
    QAction *actionNewDescriprion;
    actionNewDescriprion = new QAction(this);
    actionNewDescriprion->setText(tr("Nový popis inkluze"));
    actionNewDescriprion->setShortcut(Qt::Key_N | Qt::CTRL);
    connect(actionNewDescriprion, SIGNAL(triggered()), this, SLOT(newXmlConvolution()));
    menuAnalyze->addAction(actionNewDescriprion);

    return menuAnalyze;
}

/** **********************************************************************************
 * @brief MainWindow::createMenuHelp
 * Metoda připraví menu nápovědy k ovládání programu.
 * @return menu help
 */
QMenu *MainWindow::createMenuHelp(QMenuBar *menuBar){
    // polozka Napoveda
    QMenu *menuHelp;
    menuHelp = new QMenu(menuBar);
    menuHelp->setTitle("Nápověda");

//todo připravit nápovědu
    QAction *actionHelp;
    actionHelp = new QAction(this);
    actionHelp->setText("Nápověda k aplikaci");
    actionHelp->setShortcut(Qt::Key_F1);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(openHelp()));
    menuHelp->addAction(actionHelp);

    QAction *actionAboutApp;
    actionAboutApp = new QAction(this);
    actionAboutApp->setText("O aplikaci");
    connect(actionAboutApp, SIGNAL(triggered()), this, SLOT(aboutApplication()));
    menuHelp->addAction(actionAboutApp);

    return menuHelp;
}



/** **********************************************************************************
 * Metoda zkombinuje jednotlivá svislá menu a vytvoří hlavní vodorovnou lištu aplikace
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

/** **********************************************************************************
 * Metoda připraví posuvníky pro nastavení prahů za běhu aplikace
 * @brief MainWindow::createSliderBar
 */
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
    ratioSliderLabel = new QLabel(tr("Ratio: 1"));
    connect(sliderRatio, SIGNAL(valueChanged(int)), this, SLOT(setRatioLabelValue(int)));
    sliderRatio->setDisabled(true);

    sliderKernel = new QSlider();
    sliderKernel->setOrientation(Qt::Horizontal);
    sliderKernel->setRange(3, 50);
    sliderKernel->setValue(3);
    kernelSliderLabel = new QLabel(tr("Kernel Size: 3"));
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
   // layout->addWidget(ratioWidget);
   // layout->addWidget(kernelWidget);

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
    if(filename_factory == NULL || filename_factory->atEnd()){
        openFileChooser();
    }
   if( clasificator == 0){
       startAnalyzeContour();
   }else if(clasificator == 1){
       startAnalyzeHaar();
   }else{
       QMessageBox messageBox;
       messageBox.warning(0,tr("Chyba"),tr("Špatně nastavená volba klasifikátoru."));
       messageBox.setFixedSize(500,200);
   }
}

/** **********************************************************************************
 * Metoda spustí analýzu s požadovaným počtem vláken
 * @brief MainWindow::startAnalyze
 */
void MainWindow::startAnalyzeContour(){
    try{
        Cleaner *cleaner = new Cleaner(filename_factory, convolution_descriptor, sliderThreshold->value());

        connect(cleaner, SIGNAL(showImage(QImage *, int)), this, SLOT(writeImage(QImage *, int)));
        connect(cleaner, SIGNAL(imagesProcessed(unsigned int)), this, SLOT(changeLabelCount(unsigned int)));
        connect(sliderThreshold, SIGNAL(valueChanged(int)), cleaner, SLOT(setThresh(int)));

        cleaner->start();

    }catch(EmptyImageException &e){
        std::cout << e.what() << std::endl;
    }catch(std::exception &e){
        std::cout << "unnamed exception" << "\n";
        std::cout << e.what() << "\n";
    }
}

/** **********************************************************************************
 * Metoda spustí analýzu s požaovaným počtem vláken
 * @brief MainWindow::startAnalyze
 */
void MainWindow::startAnalyzeHaar(){
    try{
        std::string classifierFile = loadClassifierFile();
        HaarFinder *clasifier = new HaarFinder(filename_factory, classifierFile);

        connect(clasifier, SIGNAL(showImage(QImage *, int)), this, SLOT(writeImage(QImage *, int)));
        connect(clasifier, SIGNAL(imagesProcessed(unsigned int)), this, SLOT(changeLabelCount(unsigned int)));

        clasifier->start();

    }catch(FileNotAcceptableException &e){
        std::cout << e.what() << std::endl;
    }catch(std::exception &e){
        std::cout << "unnamed exception" << "\n";
        std::cout << e.what() << "\n";
    }
}

void MainWindow::changeLabelCount(unsigned int count){
    this->countL->setText(QString::number(count));
    this->countL->update();
}

/** **********************************************************************************
 * Otevre dialog pro vyber souboru
 * @brief MainWindow::openFileChooser
 * @return jmeno vybrane slozky
 */
void MainWindow::openFileChooser() {
    QString folder = QFileDialog::getExistingDirectory(this, tr("Vybrat složku s daty"),
                     QDir::currentPath(), QFileDialog::DontResolveSymlinks);

    if (!QString::compare(folder, "")) {
        QMessageBox messageBox;
        messageBox.warning(0,tr("Data"),tr("Je třeba vybrat složku s daty!"));
        messageBox.setFixedSize(500,200);
        openFileChooser();
    }

    if(filename_factory == NULL || filename_factory->atEnd()){
        filename_factory = new FilenameFactory(folder);
        this->setWindowTitle(*APP_NAME+" - ("+folder+")");
    }
}

std::string MainWindow::loadClassifierFile(){
    QString folder = QFileDialog::getOpenFileName(this, tr("Otevři popis klasifikátoru"),
                                                  QDir::currentPath(), tr("XML Files (*.xml)"));

    if (!QString::compare(folder, "")) {
        return "";
    }
    return folder.toStdString();
}

/** *******************************************************
 * Zobrazi informace o aplikaci
 * @brief MainWindow::aboutApplication
 */
void MainWindow::aboutApplication() {
    QMessageBox::about(this, tr("O aplikaci"),
             tr("<b>Aplikace pro detekci inkluzí</b> <br><br> Verze 1.0 "
                "<br> Autoři: Denisa Tarantíková, Radek Vais <br> "
                "Vytvořeno v rámci předmětu KIV/ZVI na ZČU v akademickém roce 2015/2016"));
}

/**
 * Zobrazi okno s napovedou k aplikaci
 * @brief MainWindow::openHelp
 */
void MainWindow::openHelp() {
    new HelpWindow();
}

/** **********************************************************************************
 * Dotaz pred zavrenim aplikace
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    int reply = QMessageBox::question(this, "Ukončit aplikaci", "Opravdu chcete ukončit aplikaci?", "Ano", "Ne");
    if (reply == 0) {
        QApplication::closeAllWindows();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::changeClasificator(int clasificator){
    if(clasificator == 0){
        sliderThreshold->setEnabled(true);
    }else{
        sliderThreshold->setEnabled(false);
    }
    this->clasificator = clasificator;
}

/** **********************************************************************************
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


/** **********************************************************************************
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


/** **********************************************************************************
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

/** **********************************************************************************
 * Zprostředkuje uložení xml popisu vlastností inkluze
 * @brief MainWindow::saveXmlConvolution
 */
void MainWindow::saveXmlConvolution(){
    if(convolution_descriptor == NULL){
        QMessageBox messageBox;
        messageBox.information(0,"",tr("Není připravený popis k uložení."));
        messageBox.setFixedSize(500,200);
        return;
    }
    QString path;

    if(!QString::compare(convolution_descriptor->getFILE_NAME(), "")){
        path = QFileDialog::getSaveFileName(this, tr("Uložení popisu inkluze"),
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


/** **********************************************************************************
 * Zprostředkuje načtení xml popisu vlastností inkluze
 * @brief MainWindow::loadXmlConvolution
 */
void MainWindow::loadXmlConvolution(){
    QString folder = QFileDialog::getOpenFileName(this, tr("Otevři popis konvolucí"),
                                                  QDir::currentPath(), tr("XML Files (*.xml)"));

    if (!QString::compare(folder, "")) {
        QMessageBox messageBox;
        messageBox.information(0,"",tr("Nebyl zvolen soubor"));
        messageBox.setFixedSize(500,200);
        return;
    }

    // todo kontrola, zda nebezi analyza
    bool opened = convolution_descriptor->open(folder);


    if(!opened){
        if (!QString::compare(folder, "")) {
            QMessageBox messageBox;
            messageBox.critical(0,"","Soubor "+folder+" není čitelný");
            messageBox.setFixedSize(500,200);
        }
    }else{
        description_dialog->mainTab->updateContent();
        description_dialog->show();
        QMessageBox messageBox;
        messageBox.information(0,"","Soubor "+folder+" byl načten");
        messageBox.setFixedSize(500,200);

    }

    return;
}



/** **********************************************************************************
 * Smaže adresu ukládaného souboru... Půjde tedy uložit znovu
 * @brief MainWindow::loadXmlConvolution
 */
void MainWindow::newXmlConvolution(){

    // todo kontrola, zda nebezi analyza
    convolution_descriptor->reset();

    description_dialog->mainTab->updateContent();
    description_dialog->show();

    return;
}


/** **********************************************************************************
 * Zobrazí dialog pro nastavení parametrů analýzy.
 * @brief MainWindow::showSetUp
 */
void MainWindow::showSetUp(){
    description_dialog->show();
}

/** **********************************************************************************
 * Destruktor hlavního okna
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
