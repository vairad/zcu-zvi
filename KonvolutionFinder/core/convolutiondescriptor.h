#ifndef CONVOLUTIONDESCRIPTOR_H
#define CONVOLUTIONDESCRIPTOR_H

#include <climits>

#include <QtXml>
#include <QString>

class ConvolutionDescriptor
{
    double reqAspectRatio = 1.0; //podlouhlost
    double epsilonRatio = 0.2;

    int reqMinVerticies = 1; // minimum vrcholů aproximace
    int reqMaxVerticies = INT_MAX; // maximum vrcholů aproximace

    double reqExtent = 0.2; // pravoúhlost
    double epsilonExtent = 0.2;

    QString note;

    QString FILE_NAME;
    bool prepared;

public:
    ConvolutionDescriptor();
    ConvolutionDescriptor(QString path);

    void save(QString path);
    bool isOk();

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
};

#endif // CONVOLUTIONDESCRIPTOR_H
