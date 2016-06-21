#ifndef CONVOLUTIONDESCRIPTOR_H
#define CONVOLUTIONDESCRIPTOR_H

#include <climits>

#include <QtXml>
#include <QString>

class ConvolutionDescriptor
{
    double reqAspectRatio = 1.0; //podlouhlost
    double epsilonRtatio = 0.2;

    unsigned int reqMinVerticies = 1; // minimum vrcholů aproximace
    unsigned int reqMaxVerticies = UINT_MAX; // maximum vrcholů aproximace

    double reqExtent = 0.2; // pravoúhlost
    double epsilonExtent = 0.2;

    bool prepared;

public:
    ConvolutionDescriptor(QString path);

    void save(QString path);
    bool isOk();

    double getReqAspectRatio() const;
    double getEpsilonRtatio() const;
    unsigned int getReqMinVerticies() const;
    unsigned int getReqMaxVerticies() const;
    double getReqExtent() const;
    double getEpsilonExtent() const;
};

#endif // CONVOLUTIONDESCRIPTOR_H
