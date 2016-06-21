#ifndef DESCRIPTORDIALOG_H
#define DESCRIPTORDIALOG_H


#include <QtWidgets>
#include <QDialog>

#include "core/convolutiondescriptor.h"

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;

/**
 * List hlavni - nastaveni udaju uzivatele
 * @brief The MainTab class
 */
class ConvolutionTab : public QWidget {
    Q_OBJECT

    QGroupBox *createAspectRatioBox();
    QGroupBox *createVerticiesBox();

    ConvolutionDescriptor *convolution_descriptor;

    QGroupBox *createExtentBox();
public:
    explicit ConvolutionTab(ConvolutionDescriptor *convolution_descriptor, QWidget *parent = 0);
    /** textove pole pro jmeno */
    QDoubleSpinBox *aspectRatio = NULL;
    /** textove pole pro prijmeni */
    QDoubleSpinBox *epsilonRatio = NULL;
    /** textove pole pro uzivatelske jmeno */
    QSpinBox *minVerticies = NULL;
    /** textove pole pro vek */
    QSpinBox *maxVerticies = NULL;
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

    void updatePath();
};


/**
 * Dialog umoznujici nastaveni meta dat
 * @brief The TabDialog class
 */
class DescriptionDialog : public QDialog {
    Q_OBJECT

public:
    explicit DescriptionDialog(ConvolutionDescriptor *convolution_descriptor, QWidget *parent = 0);
    /** hlavni list */
    ConvolutionTab *mainTab;

    /** spravce dat */
    ConvolutionDescriptor *convolution_descriptor;

public slots:
    void updateData();

private:
    /** widget s listy */
    QTabWidget *tabWidget;
    /** box s tlacitky OK a Cancel */
    QDialogButtonBox *buttonBox;
    /** true pokud jsou validni vstupy od uzivatele */
    bool isValidInputs;
};

#endif // DESCRIPTORDIALOG_H
