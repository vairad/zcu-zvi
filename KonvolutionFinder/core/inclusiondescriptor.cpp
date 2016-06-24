#include "core/inclusiondescriptor.h"
#include "core/exception.h"

#include <iostream>

#include <QString>
#include <QDomNode>
#include <QDomElement>
#include <QDomText>
#include <QXmlStreamWriter>

//=======================================================================================

double InclusionDescriptor::getReqAspectRatio() const{
    return reqAspectRatio;
}

double InclusionDescriptor::getEpsilonRatio() const{
    return epsilonRatio;
}

int InclusionDescriptor::getReqMinVertices() const{
    return reqMinVertices;
}

int InclusionDescriptor::getReqMaxVertices() const{
    return reqMaxVertices;
}

double InclusionDescriptor::getReqExtent() const{
    return reqExtent;
}

double InclusionDescriptor::getEpsilonExtent() const{
    return epsilonExtent;
}

QString InclusionDescriptor::getFILE_NAME() const{
    return FILE_NAME;
}

QString InclusionDescriptor::getNote() const{
    return note;
}


//=======================================================================================

void InclusionDescriptor::setReqAspectRatio(double value)
{
    reqAspectRatio = value;
}

void InclusionDescriptor::setEpsilonRatio(double value)
{
    epsilonRatio = value;
}

void InclusionDescriptor::setReqMinVertices(int value){
    reqMinVertices = value;
}

void InclusionDescriptor::setReqMaxVertices(int value)
{
    reqMaxVertices = value;
}

void InclusionDescriptor::setReqExtent(double value)
{
    reqExtent = value;
}

void InclusionDescriptor::setEpsilonExtent(double value)
{
    epsilonExtent = value;
}

void InclusionDescriptor::setNote(const QString &value)
{
    note = value;
}

void InclusionDescriptor::setFILE_NAME(const QString &value)
{
    FILE_NAME = value;
}


//=======================================================================================


bool InclusionDescriptor::getBoolMinVertices() const
{
    return boolMinVertices;
}

void InclusionDescriptor::setBoolMinVertices(bool value)
{
    boolMinVertices = value;
}

bool InclusionDescriptor::getBoolMaxVertices() const
{
    return boolMaxVertices;
}

void InclusionDescriptor::setBoolMaxVertices(bool value)
{
    boolMaxVertices = value;
}

bool InclusionDescriptor::getBoolAspectRatio() const
{
    return boolAspectRatio;
}

void InclusionDescriptor::setBoolAspectRatio(bool value)
{
    boolAspectRatio = value;
}

bool InclusionDescriptor::getBoolExtent() const
{
    return boolExtent;
}

void InclusionDescriptor::setBoolExtent(bool value)
{
    boolExtent = value;
}

//=====================================================================================

InclusionDescriptor::InclusionDescriptor(){
    FILE_NAME = "";
    boolAspectRatio = true;
    reqAspectRatio = 1.0; //podlouhlost
    epsilonRatio = 0.2;

    boolMinVertices = false;
    reqMinVertices = 1; // minimum vrcholů aproximace

    boolMaxVertices = false;
    reqMaxVertices = INT_MAX; // maximum vrcholů aproximace

    boolExtent = false;
    reqExtent = 1.0; // pravoúhlost
    epsilonExtent = 2.0;
}

bool InclusionDescriptor::open(QString path)
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
                reqMinVertices = e.text().toInt();
            }else if(e.tagName() == "MaxVerticies"){
                reqMaxVertices = e.text().toInt();
            }else if(e.tagName() == "Extent"){
                 reqExtent = e.text().toDouble();
            }else if(e.tagName() == "EpsilonExtent"){
                epsilonExtent = e.text().toDouble();
            }else if(e.tagName() == "Note"){
                note = e.text();
            }else if(e.tagName() == "BoolAspectRatio"){
                boolAspectRatio = (bool)e.text().toInt();
            }else if(e.tagName() == "BoolMinVerticies"){
                boolMinVertices= (bool)e.text().toInt();
            }else if(e.tagName() == "BoolMaxVerticies"){
                boolMaxVertices = (bool)e.text().toInt();
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
 * @brief MainWindow::loadXmlinclusion
 */
void InclusionDescriptor::reset(){
    FILE_NAME = "";
}

void InclusionDescriptor::save(QString path)
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

         outXml.writeTextElement("MinVertices", QString::number(reqMinVertices) );
         outXml.writeTextElement("BoolMinVertices", QString::number(boolMinVertices));

         outXml.writeTextElement("MaxVertices",  QString::number(reqMaxVertices) );
         outXml.writeTextElement("BoolMaxVertices", QString::number(boolMaxVertices));

         outXml.writeTextElement("Extent", QString::number(reqExtent ));
         outXml.writeTextElement("EpsilonExtent",  QString::number(epsilonExtent) );
         outXml.writeTextElement("BoolExtent", QString::number(boolExtent));

         outXml.writeTextElement("Note",  note );

     outXml.writeEndElement();

     fileToSave.close();

}
