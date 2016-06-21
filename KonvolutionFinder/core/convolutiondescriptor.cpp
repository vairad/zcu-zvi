#include "convolutiondescriptor.h"
#include "exception.h"

#include <iostream>

#include <QString>
#include <QDomNode>
#include <QDomElement>
#include <QDomText>
#include <QXmlStreamWriter>

//=======================================================================================

double ConvolutionDescriptor::getReqAspectRatio() const{
    return reqAspectRatio;
}

double ConvolutionDescriptor::getEpsilonRatio() const{
    return epsilonRatio;
}

int ConvolutionDescriptor::getReqMinVerticies() const{
    return reqMinVerticies;
}

int ConvolutionDescriptor::getReqMaxVerticies() const{
    return reqMaxVerticies;
}

double ConvolutionDescriptor::getReqExtent() const{
    return reqExtent;
}

double ConvolutionDescriptor::getEpsilonExtent() const{
    return epsilonExtent;
}

QString ConvolutionDescriptor::getFILE_NAME() const{
    return FILE_NAME;
}

QString ConvolutionDescriptor::getNote() const{
    return note;
}


//=======================================================================================

void ConvolutionDescriptor::setReqAspectRatio(double value)
{
    reqAspectRatio = value;
}

void ConvolutionDescriptor::setEpsilonRatio(double value)
{
    epsilonRatio = value;
}

void ConvolutionDescriptor::setReqMinVerticies(int value)
{
    reqMinVerticies = value;
}

void ConvolutionDescriptor::setReqMaxVerticies(int value)
{
    reqMaxVerticies = value;
}

void ConvolutionDescriptor::setReqExtent(double value)
{
    reqExtent = value;
}

void ConvolutionDescriptor::setEpsilonExtent(double value)
{
    epsilonExtent = value;
}

void ConvolutionDescriptor::setNote(const QString &value)
{
    note = value;
}

void ConvolutionDescriptor::setFILE_NAME(const QString &value)
{
    FILE_NAME = value;
}


//=======================================================================================


ConvolutionDescriptor::ConvolutionDescriptor(){
    FILE_NAME = "";
    reqAspectRatio = 1.0; //podlouhlost
    epsilonRatio = 0.2;

    reqMinVerticies = 1; // minimum vrcholů aproximace
    reqMaxVerticies = INT_MAX; // maximum vrcholů aproximace

    reqExtent = 0.2; // pravoúhlost
    epsilonExtent = 0.2;
}

bool ConvolutionDescriptor::open(QString path)
{
    QDomDocument xml("mydocument");

    QFile fileToRead(path);
    fileToRead.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!fileToRead.isReadable()){
        return false;
    }

    if (!xml.setContent(&fileToRead)) {
        return false;
    }

    fileToRead.close();

    QDomElement docElem = xml.documentElement(); // root element

    QDomNode n = docElem.firstChild();
 //   QDomNodeList list = docElem.elementsByTagName("ConvolutionDescription");
//    QDomNode n = list.at(0);
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if(e.tagName() == "AspectRatio"){
                reqAspectRatio = e.text().toDouble();
            }else if(e.tagName() == "EpsilonRatio"){
                epsilonRatio = e.text().toDouble();
            }else if(e.tagName() == "MinVerticies"){
                reqMinVerticies = e.text().toInt();
            }else if(e.tagName() == "MaxVerticies"){
                reqMaxVerticies = e.text().toInt();
            }else if(e.tagName() == "Extent"){
                 reqExtent = e.text().toDouble();
            }else if(e.tagName() == "EpsilonExtent"){
                epsilonExtent = e.text().toDouble();
            }else if(e.tagName() == "Note"){
                note = e.text();
            }
        }
        n = n.nextSibling();
    }

    //todo kontrola zda byly načteny hodnoty korektně

    FILE_NAME = path;
    return true;
}

/** **********************************************************************************
 * Smaže adresu ukládaného souboru... Půjde tedy uložit znovu
 * @brief MainWindow::loadXmlConvolution
 */
void ConvolutionDescriptor::reset(){
    FILE_NAME = "";
}

void ConvolutionDescriptor::save(QString path)
{
    QFile fileToSave(path);
    fileToSave.open(QIODevice::WriteOnly);

    if(!fileToSave.isWritable()){
        throw FileNotWriteableException(path.toStdString().c_str());
    }

     QXmlStreamWriter outXml(&fileToSave);
     outXml.setAutoFormatting(true);

     outXml.writeStartDocument();

     outXml.writeStartElement("ConvolutionDescription");

         QString number = QString::number(reqAspectRatio);
         outXml.writeTextElement("AspectRatio", number);

         outXml.writeTextElement("EpsilonRatio", QString::number(epsilonRatio) );

         outXml.writeTextElement("MinVerticies", QString::number(reqMinVerticies) );
         outXml.writeTextElement("MaxVerticies",  QString::number(reqMaxVerticies) );

         outXml.writeTextElement("Extent", QString::number(reqExtent ));
         outXml.writeTextElement("EpsilonExtent",  QString::number(epsilonExtent) );

         outXml.writeTextElement("Note",  note );

     outXml.writeEndElement();

     fileToSave.close();

}
