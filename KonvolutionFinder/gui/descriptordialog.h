#ifndef DESCRIPTORDIALOG_H
#define DESCRIPTORDIALOG_H


#include <QtWidgets>
#include <QDialog>

#include "core/inclusiondescriptor.h"

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;

/**
 * List hlavni - nastaveni udaju uzivatele
 * @brief The MainTab class
 */
class inclusionTab : public QWidget {
    Q_OBJECT

    QGroupBox *createAspectRatioBox();
    QGroupBox *createVerticesBox();
    QGroupBox *createExtentBox();

    InclusionDescriptor *inclusion_descriptor;


public:
    explicit inclusionTab(InclusionDescriptor *inclusion_descriptor, QWidget *parent = 0);
    /** textove pole pro jmeno */
    QDoubleSpinBox *aspectRatio = NULL;
    /** textove pole pro prijmeni */
    QDoubleSpinBox *epsilonRatio = NULL;
    /** textove pole pro uzivatelske jmeno */
    QSpinBox *minVertices = NULL;
    /** textove pole pro vek */
    QSpinBox *maxVertices = NULL;
    /** textove pole pro hmotnost */
    QDoubleSpinBox *extent = NULL;
    /** textove pole pro vysku */
    QDoubleSpinBox *epsilonExtent = NULL;
    /** textove pole pro poznamku */
    QTextEdit *noteTE = NULL;
    /** pole pro informaci o umisteni metadat */
    QLineEdit *pathE = NULL;
    /** label pro pole pro informaci o umisteni metadat */
    QLabel *pathL = NULL;

    QGroupBox *boxAspect;
    QGroupBox *boxExtent;
    QGroupBox *boxVertices;

    void updatePath();
    void updateContent();
};


/**
 * Dialog umoznujici nastaveni meta dat
 * @brief The TabDialog class
 */
class DescriptionDialog : public QDialog {
    Q_OBJECT

public:
    explicit DescriptionDialog(InclusionDescriptor *inclusion_descriptor, QWidget *parent = 0);
    /** hlavni list */
    inclusionTab *mainTab;

    /** spravce dat */
    InclusionDescriptor *inclusion_descriptor;

public slots:
    void updateData();

    virtual void reject();
private:
    /** widget s listy */
    QTabWidget *tabWidget;
    /** box s tlacitky OK a Cancel */
    QDialogButtonBox *buttonBox;
    /** true pokud jsou validni vstupy od uzivatele */
    bool isValidInputs;
};

#endif // DESCRIPTORDIALOG_H
