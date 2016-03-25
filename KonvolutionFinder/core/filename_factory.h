#ifndef FILENAME_FACTORY_H
#define FILENAME_FACTORY_H

#include <QDir>
#include <QString>
#include <QStringList>
#include <QStringListIterator>

class FilenameFactory
{
    /** prohledavana složka */
    QString folder_path;
    /** seznam jmen ve složce */
    QStringList name_list;
    /** seznam filtru nazvu souboru */
    QStringList filters;

    //dynamic objects
    /** Qt objekt slozky */
    QDir *folder;
    /** iterator seznamem jmen */
    QStringListIterator *name_iterator;

public:
    FilenameFactory(QString folder_path);
    ~FilenameFactory();

    void addFilePatternFilter(QString filter_pattern);
    void setFolderPath(QString folder_path);

    void initializeFactory();

    QString getNextImageName();
    QString getNextImagePath();
};

#endif // FILENAME_FACTORY_H