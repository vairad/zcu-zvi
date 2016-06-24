#ifndef FILENAME_FACTORY_H
#define FILENAME_FACTORY_H

#include <QDir>
#include <QString>
#include <QStringList>
#include <QStringListIterator>
#include <QSemaphore>

class FilenameFactory
{
    QSemaphore *runLock = new QSemaphore(1);

    /** prohledavana složka */
    QString folder_path;
    /** seznam jmen ve složce */
    QStringList name_list;
    /** seznam filtru nazvu souboru */
    QStringList filters;

    //dynamic objects
    /** Qt objekt slozky */
    QDir *folder = NULL;
    /** iterator seznamem jmen */
    QStringListIterator *name_iterator = NULL;

public:
    FilenameFactory(QString folder_path);
    ~FilenameFactory();

    void addFilePatternFilter(QString filter_pattern);
    void setFolderPath(QString folder_path);

    void initializeFactory();

    QString getNextImageName();
    QString getNextImageRelativePath();
    QString getNextImagePath();
    bool atEnd();
    void pause();
    void resume();
};

#endif // FILENAME_FACTORY_H
