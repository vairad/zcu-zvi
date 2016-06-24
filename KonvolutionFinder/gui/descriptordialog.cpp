#include <climits>

#include "gui/descriptordialog.h"


/** ************************************************************************************************
 * Konstruktor připraví dialog pro nastavení dat k analýze.
 * @brief DescriptionDialog::DescriptionDialog
 * @param inclusion_descriptor
 * @param parent
 */
DescriptionDialog::DescriptionDialog(InclusionDescriptor *inclusion_descriptor, QWidget *parent) :
                                                    QDialog(parent), inclusion_descriptor(inclusion_descriptor) {
    tabWidget = new QTabWidget;
    mainTab = new inclusionTab(this->inclusion_descriptor);
    tabWidget->addTab(mainTab, tr("Popis inkluze"));

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

void DescriptionDialog::reject(){
    mainTab->updateContent();
    QDialog::reject();
}

/** ************************************************************************************************
 * @brief DescriptionDialog::updateData
 */
void DescriptionDialog::updateData() {
    inclusion_descriptor->setEpsilonExtent(mainTab->epsilonExtent->value());
    inclusion_descriptor->setEpsilonRatio(mainTab->epsilonRatio->value());
    inclusion_descriptor->setNote(mainTab->noteTE->toPlainText());
    inclusion_descriptor->setReqAspectRatio(mainTab->aspectRatio->value());
    inclusion_descriptor->setReqExtent(mainTab->extent->value());
    inclusion_descriptor->setReqMaxVertices(mainTab->maxVertices->value());
    inclusion_descriptor->setReqMinVertices(mainTab->minVertices->value());
    inclusion_descriptor->setBoolAspectRatio(mainTab->boxAspect->isChecked());
    inclusion_descriptor->setBoolExtent(mainTab->boxExtent->isChecked());
    inclusion_descriptor->setBoolMaxVertices(mainTab->boxVertices->isChecked());
    inclusion_descriptor->setBoolMinVertices(mainTab->boxVertices->isChecked());
}


//===========================================================================================

/** ************************************************************************************************
 * @brief inclusionTab::createAspectRatioBox
 * @return
 */
QGroupBox *inclusionTab::createAspectRatioBox(){
    QLabel *ratioL = new QLabel(tr("Poměr stran:"));
    aspectRatio = new QDoubleSpinBox();
    aspectRatio->setMaximum(0);
    aspectRatio->setMaximum(30);
    aspectRatio->setSingleStep(0.01);
    aspectRatio->setValue(inclusion_descriptor->getReqAspectRatio());

    QLabel *epsRL = new QLabel(tr("Tolerance:"));
    epsilonRatio = new QDoubleSpinBox();
    epsilonRatio->setMaximum(0);
    epsilonRatio->setMaximum(2);
    epsilonRatio->setSingleStep(0.01);
    epsilonRatio->setValue(inclusion_descriptor->getEpsilonRatio());

    QHBoxLayout *ratioLayout = new QHBoxLayout();

    ratioLayout->addWidget(ratioL);
    ratioLayout->addWidget(aspectRatio);

    QHBoxLayout *epsLayout = new QHBoxLayout();

    epsLayout->addWidget(epsRL);
    epsLayout->addWidget(epsilonRatio);

    QVBoxLayout *ratioBoxLayout = new QVBoxLayout();

    ratioBoxLayout->addItem(ratioLayout);
    ratioBoxLayout->addItem(epsLayout);

    QGroupBox *box = new QGroupBox(tr("Poměr stran opsaného rovnoběžníku"));
    box->setCheckable(true);
    box->setChecked(inclusion_descriptor->getBoolAspectRatio());
    box->setLayout(ratioBoxLayout);

    return box;
}

/** ************************************************************************************************
 * @brief inclusionTab::createVerticesBox
 * @return
 */
QGroupBox *inclusionTab::createVerticesBox(){
    QLabel *minimumL = new QLabel(tr("Minimum:"));
    minVertices = new QSpinBox();
    minVertices->setMinimum(1);
    minVertices->setMaximum(INT_MAX);
    minVertices->setValue(inclusion_descriptor->getReqMinVertices());

    QLabel *maximumL = new QLabel(tr("Maximum:"));
    maxVertices = new QSpinBox();
    maxVertices->setRange(1, INT_MAX);
    maxVertices->setValue(inclusion_descriptor->getReqMaxVertices());

    QHBoxLayout *minLayout = new QHBoxLayout();

    minLayout->addWidget(minimumL);
    minLayout->addWidget(minVertices);

    QHBoxLayout *maxLayout = new QHBoxLayout();

    maxLayout->addWidget(maximumL);
    maxLayout->addWidget(maxVertices);

    QVBoxLayout *verticesBoxLayout = new QVBoxLayout();

    verticesBoxLayout->addItem(minLayout);
    verticesBoxLayout->addItem(maxLayout);

    QGroupBox *box = new QGroupBox("Počet vrcholů");
    box->setLayout(verticesBoxLayout);
    box->setCheckable(true);
    box->setChecked(inclusion_descriptor->getBoolMinVertices());

    return box;
}

/** ************************************************************************************************
 * @brief inclusionTab::createExtentBox
 * @return
 */
QGroupBox *inclusionTab::createExtentBox(){
    QLabel *extentL = new QLabel(tr("Poměr délek hranic:"));
    extent = new QDoubleSpinBox();
    extent->setMaximum(0);
    extent->setMaximum(30);
    extent->setSingleStep(0.01);
    extent->setValue(inclusion_descriptor->getReqExtent());


    QLabel *epsEL = new QLabel(tr("Tolerance:"));
    epsilonExtent = new QDoubleSpinBox();
    epsilonExtent->setMaximum(0);
    epsilonExtent->setMaximum(2);
    epsilonExtent->setSingleStep(0.01);
    epsilonExtent->setValue(inclusion_descriptor->getEpsilonExtent());

    QHBoxLayout *extentLayout = new QHBoxLayout();

    extentLayout->addWidget(extentL);
    extentLayout->addWidget(extent);

    QHBoxLayout *epsLayout = new QHBoxLayout();

    epsLayout->addWidget(epsEL);
    epsLayout->addWidget(epsilonExtent);

    QVBoxLayout *ratioBoxLayout = new QVBoxLayout();

    ratioBoxLayout->addItem(extentLayout);
    ratioBoxLayout->addItem(epsLayout);

    QGroupBox *box = new QGroupBox(tr("Poměr plochy nalezeného objektu a jemu opsaného rovnoběžníku"));
    box->setLayout(ratioBoxLayout);
    box->setCheckable(true);
    box->setChecked(inclusion_descriptor->getBoolExtent());

    return box;
}

/** ************************************************************************************************
 * @brief inclusionTab::updatePath
 */
void inclusionTab::updatePath(){
    QFileInfo fileInfo(inclusion_descriptor->getFILE_NAME());

    if(pathL == NULL){
        pathL = new QLabel(tr("Cesta k načtenému xml:"));
    }

    if(pathE == NULL){
        pathE = new QLineEdit(fileInfo.absoluteFilePath());
    }
    pathE->setText(fileInfo.absoluteFilePath());
    pathE->setReadOnly(true);

    // pokud se username v dataManageru rovna "", potom skryt cestu k metadatum
    if (!QString::compare(inclusion_descriptor->getFILE_NAME(), "")) {
        pathL->hide();
        pathE->hide();
    }else{
        pathL->setVisible(true);
        pathE->setVisible(true);
    }
}

void inclusionTab::updateContent(){
    updatePath();
    epsilonExtent->setValue(inclusion_descriptor->getEpsilonExtent());
    extent->setValue(inclusion_descriptor->getReqExtent());
    maxVertices->setValue(inclusion_descriptor->getReqMaxVertices());
    minVertices->setValue(inclusion_descriptor->getReqMinVertices());
    epsilonRatio->setValue(inclusion_descriptor->getEpsilonRatio());
    aspectRatio->setValue(inclusion_descriptor->getReqAspectRatio());
    noteTE->setText(inclusion_descriptor->getNote());
    boxAspect->setChecked(inclusion_descriptor->getBoolAspectRatio());
    boxExtent->setChecked(inclusion_descriptor->getBoolExtent());
    boxVertices->setChecked(inclusion_descriptor->getBoolMaxVertices());
}

/** ************************************************************************************************
 * Vytvori list hlavni - nastaveni udaju uzivatele
 * @brief MainTab::MainTab
 * @param fileInfo
 * @param parent
 */
inclusionTab::inclusionTab(InclusionDescriptor *inclusion_descriptor, QWidget *parent) :
                                                        QWidget(parent), inclusion_descriptor(inclusion_descriptor) {

    QLabel *noteL = new QLabel(tr("Poznámka:"));
    noteTE = new QTextEdit(inclusion_descriptor->getNote());


    QVBoxLayout *mainLayout = new QVBoxLayout;

    boxAspect = createAspectRatioBox();
    boxExtent = createExtentBox();
    boxVertices = createVerticesBox();
    mainLayout->addWidget(boxAspect);
    mainLayout->addWidget(boxVertices);
    mainLayout->addWidget(boxExtent);

    mainLayout->addWidget(noteL);
    mainLayout->addWidget(noteTE);

    updatePath();

    mainLayout->addWidget(pathL);
    mainLayout->addWidget(pathE);


    mainLayout->addStretch(2);
    setLayout(mainLayout);
}

