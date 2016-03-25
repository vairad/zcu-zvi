#include "testclass.h"

#include <iostream>

/**
 * Vytiskne na konzoli všechny názvy souborů dle předloženého továrního objektu.
 * @brief TestClass::testFilenameFactory
 * @param factory
 */
void TestConsole::testFilenameFactory(FilenameFactory *factory){
    QString name;
    name = factory->getNextImageName();
    while(name != NULL){
        std::cout << name.toStdString() << "\n";
        name = factory->getNextImagePath();
    }
}
