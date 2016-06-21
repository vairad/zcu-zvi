#include <climits>

#include "gui/descriptordialog.h"


/**
 * Vytvori dialog umoznujici nastaveni meta dat
 * @brief MetaDialog::MetaDialog
 * @param sensors widgety senzoru
 * @param numberOfSensors pocet senzoru
 * @param parent
 */
DescriptionDialog::DescriptionDialog(ConvolutionDescriptor *convolution_descriptor, QWidget *parent) : QDialog(parent), convolution_descriptor(convolution_descriptor) {
    tabWidget = new QTabWidget;
    mainTab = new ConvolutionTab(this->convolution_descriptor);
    tabWidget->addTab(mainTab, tr("Popis konvoluce"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // ulozeni metadat do Data manageru
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(updateData()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Nastavení"));
}

/**
 * Aktualizace dat v data manageru
 * @brief MetaDialog::updateMetadata
 */
void DescriptionDialog::updateData() {
    convolution_descriptor->setEpsilonExtent(mainTab->epsilonExtent->value());
    convolution_descriptor->setEpsilonRatio(mainTab->epsilonRatio->value());
    convolution_descriptor->setNote(mainTab->noteTE->toPlainText());
    convolution_descriptor->setReqAspectRatio(mainTab->aspectRatio->value());
    convolution_descriptor->setReqExtent(mainTab->extent->value());
    convolution_descriptor->setReqMaxVerticies(mainTab->maxVerticies->value());
    convolution_descriptor->setReqMinVerticies(mainTab->minVerticies->value());
}

//===========================================================================================

QGroupBox *ConvolutionTab::createAspectRatioBox(){
    QLabel *ratioL = new QLabel(tr("Poměr stran:"));
    aspectRatio = new QDoubleSpinBox();
    aspectRatio->setMaximum(0);
    aspectRatio->setMaximum(30);
    aspectRatio->setSingleStep(0.01);
    aspectRatio->setValue(convolution_descriptor->getReqAspectRatio());

    QLabel *epsRL = new QLabel(tr("Tolerance:"));
    epsilonRatio = new QDoubleSpinBox();
    epsilonRatio->setMaximum(0);
    epsilonRatio->setMaximum(2);
    epsilonRatio->setSingleStep(0.01);
    epsilonRatio->setValue(convolution_descriptor->getEpsilonRatio());

    QHBoxLayout *ratioLayout = new QHBoxLayout();

    ratioLayout->addWidget(ratioL);
    ratioLayout->addWidget(aspectRatio);

    QHBoxLayout *epsLayout = new QHBoxLayout();

    epsLayout->addWidget(epsRL);
    epsLayout->addWidget(epsilonRatio);

    QVBoxLayout *ratioBoxLayout = new QVBoxLayout();

    ratioBoxLayout->addItem(ratioLayout);
    ratioBoxLayout->addItem(epsLayout);

    QGroupBox *box = new QGroupBox();
    box->setLayout(ratioBoxLayout);

    return box;
}


QGroupBox *ConvolutionTab::createVerticiesBox(){
    QLabel *minimumL = new QLabel(tr("Minimum:"));
    minVerticies = new QSpinBox();
    minVerticies->setMinimum(1);
    minVerticies->setMaximum(INT_MAX);
    minVerticies->setValue(convolution_descriptor->getReqMinVerticies());

    QLabel *maximumL = new QLabel(tr("Maximum:"));
    maxVerticies = new QSpinBox();
    maxVerticies->setRange(1, INT_MAX);
    maxVerticies->setValue(convolution_descriptor->getReqMaxVerticies());
   // maxVerticies->setMinimum(1);
  //  maxVerticies->setMaximum(INT_MAX);


    QHBoxLayout *minLayout = new QHBoxLayout();

    minLayout->addWidget(minimumL);
    minLayout->addWidget(minVerticies);

    QHBoxLayout *maxLayout = new QHBoxLayout();

    maxLayout->addWidget(maximumL);
    maxLayout->addWidget(maxVerticies);

    QVBoxLayout *verticiesBoxLayout = new QVBoxLayout();

    verticiesBoxLayout->addItem(minLayout);
    verticiesBoxLayout->addItem(maxLayout);

    QGroupBox *box = new QGroupBox();
    box->setLayout(verticiesBoxLayout);
    return box;
}

QGroupBox *ConvolutionTab::createExtentBox(){
    QLabel *extentL = new QLabel(tr("Poměr délek hranic:"));
    extent = new QDoubleSpinBox();
    extent->setMaximum(0);
    extent->setMaximum(30);
    extent->setSingleStep(0.01);
    extent->setValue(convolution_descriptor->getReqExtent());


    QLabel *epsEL = new QLabel(tr("Tolerance:"));
    epsilonExtent = new QDoubleSpinBox();
    epsilonExtent->setMaximum(0);
    epsilonExtent->setMaximum(2);
    epsilonExtent->setSingleStep(0.01);
    epsilonExtent->setValue(convolution_descriptor->getEpsilonExtent());

    QHBoxLayout *extentLayout = new QHBoxLayout();

    extentLayout->addWidget(extentL);
    extentLayout->addWidget(extent);

    QHBoxLayout *epsLayout = new QHBoxLayout();

    epsLayout->addWidget(epsEL);
    epsLayout->addWidget(epsilonExtent);

    QVBoxLayout *ratioBoxLayout = new QVBoxLayout();

    ratioBoxLayout->addItem(extentLayout);
    ratioBoxLayout->addItem(epsLayout);

    QGroupBox *box = new QGroupBox();
    box->setLayout(ratioBoxLayout);

    return box;
}

void ConvolutionTab::updatePath(){
    QFileInfo fileInfo(convolution_descriptor->getFILE_NAME());

    if(pathL == NULL){
        pathL = new QLabel(tr("Cesta k načtenému xml:"));
    }

    if(pathE == NULL){
        pathE = new QLineEdit(fileInfo.absoluteFilePath());
    }
    pathE->setText(fileInfo.absoluteFilePath());
    pathE->setReadOnly(true);

    // pokud se username v dataManageru rovna "", potom skryt cestu k metadatum
    if (!QString::compare(convolution_descriptor->getFILE_NAME(), "")) {
        pathL->hide();
        pathE->hide();
    }else{
        pathL->setVisible(true);
        pathE->setVisible(true);
    }
}

/**
 * Vytvori list hlavni - nastaveni udaju uzivatele
 * @brief MainTab::MainTab
 * @param fileInfo
 * @param parent
 */
ConvolutionTab::ConvolutionTab(ConvolutionDescriptor *convolution_descriptor, QWidget *parent) :
                                                        QWidget(parent), convolution_descriptor(convolution_descriptor) {

    QLabel *noteL = new QLabel(tr("Poznámka:"));
    noteTE = new QTextEdit(convolution_descriptor->getNote());


    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(createAspectRatioBox());
    mainLayout->addWidget(createVerticiesBox());
    mainLayout->addWidget(createExtentBox());

    mainLayout->addWidget(noteL);
    mainLayout->addWidget(noteTE);

    updatePath();

    mainLayout->addWidget(pathL);
    mainLayout->addWidget(pathE);


    mainLayout->addStretch(2);
    setLayout(mainLayout);
}

