#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include "core/filename_factory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QImage *lastImageOriginal = NULL;
    QImage *lastImageProcessed = NULL;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    FilenameFactory *filename_factory;

private:
    Ui::MainWindow *ui;

    void createMenuBar();
    void createImage();

    //void MainWindow::closeEvent(QCloseEvent *event);

    void writeOriginalImage(QImage *image);
    void writeNewImage(QImage *image);

private slots:
    void openFileChooser();
    void writeImage(QImage *image, int destination);

};

#endif // MAINWINDOW_H
