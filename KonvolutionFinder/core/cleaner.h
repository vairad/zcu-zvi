#ifndef CLEANER_H
#define CLEANER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "core/filename_factory.h"

class Cleaner
{


public:
    Cleaner(FilenameFactory *names);
};

#endif // CLEANER_H
