#include "core/exception.h"

EmptyImageException::EmptyImageException(const char *msg){
    this->msg = "KonvolutionFinder: Image was not loaded! \n" ;
    this->msg += "path: " ;
    this->msg += msg ;
    this->msg += "\n" ;
}

EmptyImageException::~EmptyImageException() throw(){
}

const char* EmptyImageException::what() const throw()
{
  return msg.c_str();
}


FileNotWriteableException::FileNotWriteableException(const char *msg){
    this->msg = "KonvolutionFinder: Can't write to file! \n" ;
    this->msg += "path: " ;
    this->msg += msg ;
    this->msg += "\n" ;
}

FileNotWriteableException::~FileNotWriteableException() throw(){
}

const char* FileNotWriteableException::what() const throw()
{
  return msg.c_str();
}
