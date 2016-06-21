#ifndef CONVOLUTIONDESCRIPTOR_H
#define CONVOLUTIONDESCRIPTOR_H

#include <climits>

#include <QtXml>
#include <QString>

class ConvolutionDescriptor
{
    double reqAspectRatio; //podlouhlost
    double epsilonRatio;

    int reqMinVerticies; // minimum vrcholů aproximace
    int reqMaxVerticies; // maximum vrcholů aproximace

    double reqExtent; // pravoúhlost
    double epsilonExtent;

    QString note;

    QString FILE_NAME;

public:
    ConvolutionDescriptor();
    ConvolutionDescriptor(QString path);

    void save(QString path);

    double getReqAspectRatio() const;
    double getEpsilonRatio() const;
    int getReqMinVerticies() const;
    int getReqMaxVerticies() const;
    double getReqExtent() const;
    double getEpsilonExtent() const;
    QString getFILE_NAME() const;
    QString getNote() const;
    void setReqAspectRatio(double value);
    void setEpsilonRatio(double value);
    void setReqMinVerticies(int value);
    void setReqMaxVerticies(int value);
    void setReqExtent(double value);
    void setEpsilonExtent(double value);
    void setNote(const QString &value);
    void setFILE_NAME(const QString &value);
    bool open(QString path);
    void reset();
};

#endif // CONVOLUTIONDESCRIPTOR_H
