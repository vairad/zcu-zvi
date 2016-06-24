#ifndef inclusionDESCRIPTOR_H
#define inclusionDESCRIPTOR_H

#include <climits>

#include <QtXml>
#include <QString>

class InclusionDescriptor
{
    double reqAspectRatio; //podlouhlost
    double epsilonRatio;

    int reqMinVertices; // minimum vrcholů aproximace
    int reqMaxVertices; // maximum vrcholů aproximace

    double reqExtent; // pravoúhlost
    double epsilonExtent;

    bool boolMinVertices;
    bool boolMaxVertices;
    bool boolAspectRatio;
    bool boolExtent;

    QString note;

    QString FILE_NAME;

public:
    InclusionDescriptor();
    InclusionDescriptor(QString path);

    void save(QString path);

    double getReqAspectRatio() const;
    double getEpsilonRatio() const;
    int getReqMinVertices() const;
    int getReqMaxVertices() const;
    double getReqExtent() const;
    double getEpsilonExtent() const;
    QString getFILE_NAME() const;
    QString getNote() const;
    void setReqAspectRatio(double value);
    void setEpsilonRatio(double value);
    void setReqMinVertices(int value);
    void setReqMaxVertices(int value);
    void setReqExtent(double value);
    void setEpsilonExtent(double value);
    void setNote(const QString &value);
    void setFILE_NAME(const QString &value);
    bool open(QString path);
    void reset();
    bool getBoolMinVertices() const;
    void setBoolMinVertices(bool value);
    bool getBoolMaxVertices() const;
    void setBoolMaxVertices(bool value);
    bool getBoolAspectRatio() const;
    void setBoolAspectRatio(bool value);
    bool getBoolExtent() const;
    void setBoolExtent(bool value);
};

#endif // inclusionDESCRIPTOR_H
