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


bool ConvolutionDescriptor::getBoolMinVerticies() const
{
    return boolMinVerticies;
}

void ConvolutionDescriptor::setBoolMinVerticies(bool value)
{
    boolMinVerticies = value;
}

bool ConvolutionDescriptor::getBoolMaxVerticies() const
{
    return boolMaxVerticies;
}

void ConvolutionDescriptor::setBoolMaxVerticies(bool value)
{
    boolMaxVerticies = value;
}

bool ConvolutionDescriptor::getBoolAspectRatio() const
{
    return boolAspectRatio;
}

void ConvolutionDescriptor::setBoolAspectRatio(bool value)
{
    boolAspectRatio = value;
}

bool ConvolutionDescriptor::getBoolExtent() const
{
    return boolExtent;
}

void ConvolutionDescriptor::setBoolExtent(bool value)
{
    boolExtent = value;
}

//=====================================================================================

ConvolutionDescriptor::ConvolutionDescriptor(){
    FILE_NAME = "";
    boolAspectRatio = true;
    reqAspectRatio = 1.0; //podlouhlost
    epsilonRatio = 0.2;

    boolMinVerticies = false;
    reqMinVerticies = 1; // minimum vrcholů aproximace

    boolMaxVerticies = false;
    reqMaxVerticies = INT_MAX; // maximum vrcholů aproximace

    boolExtent = false;
    reqExtent = 1.0; // pravoúhlost
    epsilonExtent = 2.0;
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
 //   QDomNodeList list = docElem.elementsByTagName("InclusionDescription");
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
            }else if(e.tagName() == "BoolAspectRatio"){
                boolAspectRatio = (bool)e.text().toInt();
            }else if(e.tagName() == "BoolMinVerticies"){
                boolMinVerticies= (bool)e.text().toInt();
            }else if(e.tagName() == "BoolMaxVerticies"){
                boolMaxVerticies = (bool)e.text().toInt();
            }else if(e.tagName() == "BoolExtent"){
                boolExtent = (bool)e.text().toInt();
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

     outXml.writeStartElement("InclusionDescription");

         QString number = QString::number(reqAspectRatio);

         outXml.writeTextElement("AspectRatio", number);
         outXml.writeTextElement("EpsilonRatio", QString::number(epsilonRatio) );
         outXml.writeTextElement("BoolAspectRatio", QString::number(boolAspectRatio));

         outXml.writeTextElement("MinVerticies", QString::number(reqMinVerticies) );
         outXml.writeTextElement("BoolMinVerticies", QString::number(boolMinVerticies));

         outXml.writeTextElement("MaxVerticies",  QString::number(reqMaxVerticies) );
         outXml.writeTextElement("BoolMaxVerticies", QString::number(boolMaxVerticies));

         outXml.writeTextElement("Extent", QString::number(reqExtent ));
         outXml.writeTextElement("EpsilonExtent",  QString::number(epsilonExtent) );
         outXml.writeTextElement("BoolExtent", QString::number(boolExtent));

         outXml.writeTextElement("Note",  note );

     outXml.writeEndElement();

     fileToSave.close();

}
