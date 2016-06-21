#include <exception>
#include <string>

#ifndef EXCEPTION_H
#define EXCEPTION_H

/**
 * Výjimka, která symbolizuje nenačtený obrázek
 *
 * @author Radek VAIS - A13B0457P
 * 25.3.2016
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
 * @author Radek VAIS - A13B0457P
 * 25.3.2016
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


#endif // EXCEPTION_H
