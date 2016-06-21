#include "convolutiondescriptor.h"
#include "exception.h"

#include <iostream>
#include <QString>

double ConvolutionDescriptor::getReqAspectRatio() const
{
    return reqAspectRatio;
}

double ConvolutionDescriptor::getEpsilonRtatio() const
{
    return epsilonRtatio;
}

unsigned int ConvolutionDescriptor::getReqMinVerticies() const
{
    return reqMinVerticies;
}

unsigned int ConvolutionDescriptor::getReqMaxVerticies() const
{
    return reqMaxVerticies;
}

double ConvolutionDescriptor::getReqExtent() const
{
    return reqExtent;
}

double ConvolutionDescriptor::getEpsilonExtent() const
{
    return epsilonExtent;
}

ConvolutionDescriptor::ConvolutionDescriptor(QString path)
{
    QFile fileToRead(path);
    fileToRead.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!fileToRead.isReadable()){
        throw FileNotWriteableException(path.toStdString().c_str());
    }

    QXmlStreamReader xml(&fileToRead);
    xml.readNext();

    while (!xml.atEnd()) {
      if(xml.isStartElement()) {
          if(xml.name() == "ConvolutionDescription"){
              while(!xml.atEnd()){
                if(xml.isEndElement()) {
                    xml.readNext();
                    break;
                }else if(xml.isCharacters()) {
                    xml.readNext();
                }else if(xml.isStartElement()){
                    if(xml.name() == "AspectRatio"){
                        reqAspectRatio = xml.readElementText().toDouble();
                        std::cout << "ratio" << reqAspectRatio;
                    }else if(xml.name() == "EpsilonRatio"){
                        epsilonRtatio = xml.readElementText().toDouble();
                    }else if(xml.name() == "MinVerticies"){
                        reqMinVerticies =  xml.readElementText().toUInt();
                    }else if(xml.name() == "MaxVerticies"){
                        reqMaxVerticies =  xml.readElementText().toUInt();
                    }else if(xml.name() == "reqExtent"){
                        reqExtent =  xml.readElementText().toDouble();
                    }else if(xml.name() == "epsilonExtent"){
                        epsilonExtent =  xml.readElementText().toDouble();
                    }
                }
                xml.readNext();
             }
        }
    }
    if (xml.hasError()) {
     prepared = false;
     return; // todo do error handling
    }
    xml.readNext();
    }
    prepared = true;
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

         outXml.writeTextElement("AspectRatio", QString::number(reqAspectRatio) );

         outXml.writeTextElement("EpsilonRatio", QString::number(epsilonRtatio ));

         outXml.writeTextElement("MinVerticies", QString::number(reqMinVerticies) );
         outXml.writeTextElement("MaxVerticies",  QString::number(reqMaxVerticies) );

         outXml.writeTextElement("Extent", QString::number(reqExtent ));
         outXml.writeTextElement("EpsilonExtent",  QString::number(epsilonExtent) );

     outXml.writeEndElement();

}

bool ConvolutionDescriptor::isOk(){
    return prepared;
}
