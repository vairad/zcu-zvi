#ifndef FILENAME_FACTORY_H
#define FILENAME_FACTORY_H

#include <QDir>
#include <QString>
#include <QStringList>
#include <QStringListIterator>

class FilenameFactory
{
    QStringList name_list;
    QStringListIterator *name_iterator;
public:
    FilenameFactory(QString folder_path);
    QString getNextImageName();
};

#endif // FILENAME_FACTORY_H
