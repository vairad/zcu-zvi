#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/filename_factory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    FilenameFactory *filename_factory;

private:
    Ui::MainWindow *ui;

    void MainWindow::createMenuBar();

    //void MainWindow::closeEvent(QCloseEvent *event);

private slots:
    void openFileChooser();

};

#endif // MAINWINDOW_H
