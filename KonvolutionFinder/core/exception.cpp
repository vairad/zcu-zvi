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
