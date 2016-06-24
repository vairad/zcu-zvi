#include "filename_factory.h"

/**
 * Konstruktor připraví továrnu jmen s výchozími parametry masky souborů na *.tif soubory
 * @brief FilenameFactory::FilenameFactory
 * @param folder_path QString cesta ke slozce
 */
FilenameFactory::FilenameFactory(QString folder_path) {
    this->folder_path = folder_path;

    filters.clear();
    filters << "*.tif";

    initializeFactory();
}


/**
 * Inicializuje továrnu jmen na začátek složky, abecedně řazeno.
 * @brief FilenameFactory::initializeFactory
 */
void FilenameFactory::initializeFactory(){
    if(folder != NULL){ // pokud již byla slozka nastavena... zruš puvodni instanci
        delete folder;
    }

    folder = new QDir(folder_path);
    folder->setNameFilters(filters);
    name_list = folder->entryList();

    if(name_iterator != NULL){ // pokud již byl name iterator nastaveny... zruš puvodni instanci
        delete name_iterator;
    }
    name_iterator = new QStringListIterator(name_list);
}


/**
 * Připojí další pattern do seznamu filtrů
 * @brief FilenameFactory::addFilePatternFilter
 * @param filter_pattern QString file pattern
 */
void FilenameFactory::addFilePatternFilter(QString filter_pattern){
    filters << filter_pattern;
}


/**
 * Nastavi novou hodnotu výchozí složky. Po nastavení je třeba továrnu opět inicializovat
 * @brief FilenameFactory::setFolderPath
 * @param folder_path QString folder_path
 */
void FilenameFactory::setFolderPath(QString folder_path){
    this->folder_path = folder_path;
}


/**
 * Vrati nazev nasledujiciho souboru a posune iterator
 * @brief FilenameFactory::getNextImageName
 * @return nextName of NULL
 */
QString FilenameFactory::getNextImageName() {
    if (name_iterator->hasNext()) {
       return name_iterator->next();
    } else {
        return NULL;
    }
}

/**
 * Vrati nazev nasledujiciho souboru a posune iterator
 * @brief FilenameFactory::getNextImageName
 * @return nextName of NULL
 */
QString FilenameFactory::getNextImagePath() {
    if (name_iterator->hasNext()) {
       return folder->absolutePath()+"/"+name_iterator->next();
    } else {
        return NULL;
    }
}

/**
 * Vrati relativni nazev vuci programovemu souboru nasledujiciho souboru a posune iterator
 * @brief FilenameFactory::getNextImageName
 * @return nextName of NULL
 */
QString FilenameFactory::getNextImageRelativePath() {
    runLock->acquire(); // v případě, že je zastaveno vykreslování dojde k zablokování procesu
    runLock->release(); // nejde o ochranu zdrojů, proto hned semaforuvolníme
    if (name_iterator->hasNext()) {
        QDir exeFolder(QDir::currentPath());
        return (exeFolder.relativeFilePath(folder->absolutePath())+"/"+name_iterator->next());
    } else {
        return NULL;
    }
}

/**
 * Vrati bool, zda jsou v tovarne dalsi soubory ke zpracovani.
 * @brief FilenameFactory::atEnd
 * @return true/false
 */
bool FilenameFactory::atEnd() {
    return !name_iterator->hasNext();
}

void FilenameFactory::pause(){
    runLock->acquire();
}

void FilenameFactory::resume(){
    runLock->release();
}

/**
 * Factroy destructor
 * @brief FilenameFactory::~FilenameFactory
 */
FilenameFactory::~FilenameFactory() {
    delete name_iterator;
    delete folder;
}
