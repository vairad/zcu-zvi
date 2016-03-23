#include "filename_factory.h"

#include <iostream>

FilenameFactory::FilenameFactory(QString folder_path) {
    QDir image_folder(folder_path);

    QStringList filters;
    filters << "*.tif";
    image_folder.setNameFilters(filters);

    name_list = image_folder.entryList();

    name_iterator = new QStringListIterator(name_list);
}

QString FilenameFactory::getNextImageName() {
    if (name_iterator->hasNext()) {
       return name_iterator->next();
    } else {
        return NULL;
    }
}
