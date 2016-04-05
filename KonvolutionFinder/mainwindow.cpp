#include <QtWidgets>
#include <QDir>

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
    createImage();
}

/**
 * Vytvori menu aplikace
 * @brief MainWindow::createMenuBar
 */
void MainWindow::createMenuBar() {
    QMenuBar *menuBar;
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 756, 21));

    // polozka Soubor
    QMenu *menuFile;
    menuFile = new QMenu(menuBar);
    menuFile->setTitle("Soubor");
    menuBar->addAction(menuFile->menuAction());

    // polozka Napoveda
    QMenu *menuHelp;
    menuHelp = new QMenu(menuBar);
    menuHelp->setTitle("Nápověda");
    menuBar->addAction(menuHelp->menuAction());

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

    this->setMenuBar(menuBar);
}


void MainWindow::createImage(){
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
//      filename_factory = new FilenameFactory("../data/test");

//    TestConsole::testFilenameFactory(filename_factory);
    try{
        Cleaner *cleaner = new Cleaner();
        cleaner->setFactory(filename_factory);
        connect(cleaner, SIGNAL(showImage(QImage *, int)), this, SLOT(writeImage(QImage *, int)));
        cleaner->start();

    }catch(EmptyImageException &e){
        std::cout << e.what() << std::endl;
    }catch(std::exception &e){
        std::cout << "unnamed exception" << "\n";
        std::cout << e.what() << "\n";
    }
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
 * Destruktor hlavního okna
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
