#include <exception>
#include <string>

#ifndef EXCEPTION_H
#define EXCEPTION_H

/**
 * Výjimka, která symbolizuje nenačtený obrázek
 *
 * @version verze 1.0.0
 */
class EmptyImageException : public std::exception
{
    std::string msg;
  public:
    EmptyImageException(const char *msg);
    ~EmptyImageException() throw();
    virtual const char* what() const throw();
};


/**
 * Výjimka, která symbolizuje nezapisovatelný soubor
 *
 * @version verze 1.0.0
 */
class FileNotWriteableException : public std::exception
{
    std::string msg;
  public:
    FileNotWriteableException(const char *msg);
    ~FileNotWriteableException() throw();
    virtual const char* what() const throw();
};

/**
 * Výjimka, která symbolizuje nepoužitelny soubor pro CascadeClassifier
 *
 * @version verze 1.0.0
 */
class FileNotAcceptableException : public std::exception
{
    std::string msg;
  public:
    FileNotAcceptableException(const char *msg);
    ~FileNotAcceptableException() throw();
    virtual const char* what() const throw();
};


#endif // EXCEPTION_H
