#include <QtWidgets>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/cleaner.h"
//#include "core/filename_factory.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Konvolution Finder");

    createMenuBar();
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
    Cleaner cleaner(filename_factory);
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


MainWindow::~MainWindow()
{
    delete ui;
}
