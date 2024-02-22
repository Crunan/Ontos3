#include "operatortab.h"
#include "ui_mainwindow.h"
#include "plasmacontroller.h"
#include "UtilitiesAndConstants.h"
#include "qdir.h"
#include <QApplication>
#include "mainwindow.h"

static const char lightTowerGreenStyleSheet[] = "QFrame { background-color: rgb(115, 210, 22) }";
static const char lightTowerYellowStyleSheet[] = "QFrame { background-color: rgb(252, 233, 79) }";
static const char lightTowerRedStyleSheet[] = "QFrame { background-color: rgb(204, 0, 0) }";
static const char lightTowerInactiveStyleSheet[] = "QFrame { background-color: palette(base) }";
static const char mfcFlowLinesActive[] = "border: 1px solid rgb(52, 101, 164);";
static const char mfcFlowLinesInactive[] = "border: 1px solid black;";
static const char mfcBoxActive[] = "background-color: rgb(52, 101, 164);";
static const char mfcBoxActivText[] = "color: rgb(255, 255, 255);";
static const char mfcBoxInactive[] = "background-color: rgb(base);";
static const char mfcBoxInactivText[] = "color: rgb(0, 0, 0);";


OperatorTab::OperatorTab(Ui::MainWindow* ui, PlasmaController &controller, MainWindow *mWndw, QObject* parent) :
    QObject(parent),
    m_pUI(ui),
    m_pMWndw(mWndw),
    m_controller(controller)
{
    populateRecipeComboBox();

    connect(m_pUI->btnOPChuckVacOnOff, &QPushButton::clicked, this, &OperatorTab::btnChuckVacOnOff_clicked);
    connect(m_pUI->btnOPPinsUpDown, &QPushButton::clicked, this, &OperatorTab::btnOPPinsUpDown_clicked);
    connect(m_pUI->btnOPStartPlasma, &QPushButton::clicked, this, &OperatorTab::btnOPStartPlasma_clicked);
    connect(m_pUI->btnOPHome, &QPushButton::clicked, this, &OperatorTab::btnOPHome_clicked);
    connect(m_pUI->btnOPInit, &QPushButton::clicked, this, &OperatorTab::btnOPInit_clicked);
    connect(m_pUI->comboBoxOPRecipe, &QComboBox::currentTextChanged, this, &OperatorTab::comboBoxOPRecipe_currentTextChanged);
    connect(m_pUI->comboBoxOPLogin, &QComboBox::currentTextChanged, this, &OperatorTab::comboBoxOPLogin_currentTextChanged);

    m_pUI->btnOPAcknowledge->hide();
}

OperatorTab::~OperatorTab()
{
    // do not delete here, ui object is cleaned up in MainWindow
    m_pUI = nullptr;
    m_pMWndw = nullptr;
}

void OperatorTab::populateRecipeComboBox()
{
    m_pUI->comboBoxOPRecipe->clear();

    QDir directory(RECIPE_DIRECTORY);

    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    foreach (const QFileInfo& fileInfo, fileInfoList) {
        // Add each file name to the list widget
        m_pUI->comboBoxOPRecipe->addItem(fileInfo.fileName());
    }
}

void OperatorTab::connectMFCFlowBars()
{
    for (int i = 0; i < m_controller.getMFCs().size(); ++i) {
        connect(m_controller.getMFCs()[i], &MFC::recipeFlowChanged, this, &OperatorTab::updateRecipeFlow);
        connect(m_controller.getMFCs()[i], &MFC::updateUIActualFlow, this, &OperatorTab::actualFlowChanged);
    }
}

void OperatorTab::lightTowerSetGreen()
{
    m_pUI->frameOPGreenLight->setStyleSheet(QString(lightTowerGreenStyleSheet));
    m_pUI->frameOPYellowLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
    m_pUI->frameOPRedLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
}

void OperatorTab::lightTowerSetYellow()
{
    m_pUI->frameOPGreenLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
    m_pUI->frameOPYellowLight->setStyleSheet(QString(lightTowerYellowStyleSheet));
    m_pUI->frameOPRedLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
}

void OperatorTab::lightTowerSetRed()
{
    m_pUI->frameOPGreenLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
    m_pUI->frameOPYellowLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
    m_pUI->frameOPRedLight->setStyleSheet(QString(lightTowerRedStyleSheet));
}

void OperatorTab::lightTowerSetInactive()
{
    m_pUI->frameOPGreenLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
    m_pUI->frameOPYellowLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
    m_pUI->frameOPRedLight->setStyleSheet(QString(lightTowerInactiveStyleSheet));
}


void OperatorTab::mfcFlowLinesOn(int mfcNum, bool on)
{
    QString lineColor = (on == true) ? QString(mfcFlowLinesActive) : QString(mfcFlowLinesInactive);
    QString boxColor = (on == true) ? QString(mfcBoxActive) : QString(mfcBoxInactive);
    QString textColor = (on == true) ? QString(mfcBoxActivText) : QString(mfcBoxInactivText);

    if (mfcNum == 1) {
        // set line colors
        m_pUI->lineOPMFC1pre->setStyleSheet(lineColor);
        m_pUI->lineOPMFC1post->setStyleSheet(lineColor);
        m_pUI->lineOPMFC1vert->setStyleSheet(lineColor);
        // set box and text color
        m_pUI->frameMFC1->setStyleSheet(boxColor);
        m_pUI->lblOPMFC1->setStyleSheet(textColor);
    }
    else if (mfcNum == 2) {
        // set line colors
        m_pUI->lineOPMFC2pre->setStyleSheet(lineColor);
        m_pUI->lineOPMFC2post->setStyleSheet(lineColor);
        m_pUI->lineOPMFC2vert->setStyleSheet(lineColor);
        // set box and text color
        m_pUI->frameMFC2->setStyleSheet(boxColor);
        m_pUI->lblOPMFC2->setStyleSheet(textColor);
    }
    else if (mfcNum == 3) {
        // set line colors
        m_pUI->lineOPMFC3pre->setStyleSheet(lineColor);
        m_pUI->lineOPMFC3post->setStyleSheet(lineColor);
        m_pUI->lineOPMFC3vert->setStyleSheet(lineColor);
        // set box and text color
        m_pUI->frameMFC3->setStyleSheet(boxColor);
        m_pUI->lblOPMFC3->setStyleSheet(textColor);
    }
    else if (mfcNum == 4) {
        // set line colors
        m_pUI->lineOPMFC4pre->setStyleSheet(lineColor);
        m_pUI->lineOPMFC4post->setStyleSheet(lineColor);
        m_pUI->lineOPMFC4vert->setStyleSheet(lineColor);
        // set box and text color
        m_pUI->frameMFC4->setStyleSheet(boxColor);
        m_pUI->lblOPMFC4->setStyleSheet(textColor);
    }
    else if (mfcNum == 5) {
        // set line colors
        m_pUI->lineOPMFC5pre->setStyleSheet(lineColor);
        m_pUI->lineOPMFC5post->setStyleSheet(lineColor);
        m_pUI->lineOPMFC5vert->setStyleSheet(lineColor);
        // set box and text color
        m_pUI->frameMFC5->setStyleSheet(boxColor);
        m_pUI->lblOPMFC5->setStyleSheet(textColor);
    }
    else if (mfcNum == 6) {
        // set line colors
        m_pUI->lineOPMFC6pre->setStyleSheet(lineColor);
        m_pUI->lineOPMFC6post->setStyleSheet(lineColor);
        m_pUI->lineOPMFC6vert->setStyleSheet(lineColor);
        // set box and text color
        m_pUI->frameMFC6->setStyleSheet(boxColor);
        m_pUI->lblOPMFC6->setStyleSheet(textColor);
    }
}

// configure the gasses layout according to the number of
// MFC's returned by the main controller
void OperatorTab::setUINumberOfMFCs(const int numMFCs)
{
    if (numMFCs == 5) { // if we have 5 MFC's hide MFC 6
        m_pUI->progressBarOPMFC6->hide();
        m_pUI->lineOPMFC6pre->hide();
        m_pUI->frameMFC6->hide();
        m_pUI->lineOPMFC6post->hide();
        m_pUI->lineOPMFC6vert->hide();
    }
    else if (numMFCs == 4) { // if we have 4 MFC's hide MFC 5 and 6
        m_pUI->progressBarOPMFC5->hide();
        m_pUI->lineOPMFC5pre->hide();
        m_pUI->frameMFC5->hide();
        m_pUI->lineOPMFC5post->hide();
        m_pUI->lineOPMFC5vert->hide();

        m_pUI->progressBarOPMFC6->hide();
        m_pUI->lineOPMFC6pre->hide();
        m_pUI->frameMFC6->hide();
        m_pUI->lineOPMFC6post->hide();
        m_pUI->lineOPMFC6vert->hide();
    }
}

void OperatorTab::displayAbortMessage(QString smsg)
{
    m_pUI->texteditOPTabAxisStatus->setTextColor(QColor(133, 2, 2));
    m_pUI->texteditOPTabAxisStatus->setText(smsg);
    m_pUI->btnOPAcknowledge->show();
}


//////////////// SLOTS //////////////////////////

void OperatorTab::doorStateChanged(bool state)
{
    if (state) {
        m_pUI->lblOPDoorOpen->setText("Yes");
    }
    else {
        m_pUI->lblOPDoorOpen->setText("No");
    }
}


void OperatorTab::lightTowerStateChanged(LightTower::LightState state)
{
    switch(state) {
    case LightTower::LT_INACTIVE:
        lightTowerSetInactive();
        break;
    case LightTower::LT_ERROR:
        lightTowerSetRed();
        break;
    case LightTower::LT_READY:
        lightTowerSetYellow();
        break;
    case LightTower::LT_ACTIVE:
        lightTowerSetGreen();
        break;
    }
}


void OperatorTab::axisStatusToUI()
{
    // XAxis
    if (m_controller.getAxesController().getXAxisState() >= AXIS_IDLE) {
        double Xpos = m_controller.getAxesController().getXPosition();
        m_pUI->lblOPStagePositionPH_X->setText(QString::number(m_controller.getAxesController().TranslateCoordXBase2PH(Xpos)));
    }
    else {
        m_pUI->lblOPStagePositionPH_X->setText("???");
    }
    // YAxis
    if (m_controller.getAxesController().getYAxisState() >= AXIS_IDLE) {
        double Ypos = m_controller.getAxesController().getYPosition();
        m_pUI->lblOPStagePositionPH_Y->setText(QString::number(m_controller.getAxesController().TranslateCoordYBase2PH(Ypos)));
    }
    else {
        m_pUI->lblOPStagePositionPH_Y->setText("???");
    }
    // ZAxis
    if (m_controller.getAxesController().getZAxisState() >= AXIS_IDLE) {
        double Zpos = m_controller.getAxesController().getZPosition();
        m_pUI->lblOPStagePositionPH_Z->setText(QString::number(m_controller.getAxesController().TranslateCoordZBase2PH(Zpos)));
    }
    else {
        m_pUI->lblOPStagePositionPH_Z->setText("???");
    }
}


// update the recipe progress bar and values
void OperatorTab::updateRecipeFlow(const int mfcNumber, const double recipeFlow)
{
    // This uses the parameters passed in the signal
    if (mfcNumber == 1) {
        double range = m_controller.findMFCByNumber(1)->getRange();
        int percentage = 0;
        if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
        m_pUI->progressBarOPMFC1->setValue(int(percentage));
    }
    else if (mfcNumber == 2) {
        double range = m_controller.findMFCByNumber(2)->getRange();
        int percentage = 0;
        if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
        m_pUI->progressBarOPMFC2->setValue(int(percentage));
    }
    else if (mfcNumber == 3) {
        double range = m_controller.findMFCByNumber(3)->getRange();
        int percentage = 0;
        if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
        m_pUI->progressBarOPMFC3->setValue(int(percentage));
    }
    else if (mfcNumber == 4) {
        double range = m_controller.findMFCByNumber(4)->getRange();
        int percentage = 0;
        if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
        m_pUI->progressBarOPMFC4->setValue(int(percentage));
    }
    else if (mfcNumber == 5) {
        double range = m_controller.findMFCByNumber(5)->getRange();
        int percentage = 0;
        if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
        m_pUI->progressBarOPMFC5->setValue(int(percentage));
    }
    else if (mfcNumber == 6) {
        double range = m_controller.findMFCByNumber(6)->getRange();
        int percentage = 0;
        if (range != 0 && recipeFlow != 0) percentage = int((recipeFlow / range) * 100); // divide by zero protection
        m_pUI->progressBarOPMFC6->setValue(int(percentage));
    }
}

void OperatorTab::actualFlowChanged(const int mfcNumber, const double actualFlow)
{
    // This uses the parameters passed in the signal
    if (mfcNumber == 1) {
        // set vertical progress bar
        double range = m_controller.findMFCByNumber(1)->getRange();
        int percentage = 0;
        if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

        m_pUI->lblOPMFC1ActualFlow->setText(QString::number(range, 'f', 1));

        // update the progress bar
        m_pUI->progressBarOPMFC1->setValue(int(percentage));
    }
    else if (mfcNumber == 2) {
        // set vertical progress bar
        double range = m_controller.findMFCByNumber(2)->getRange();

        int percentage = 0;
        if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

        m_pUI->lblOPMFC2ActualFlow->setText(QString::number(range, 'f', 1));

        // update the progress bar
        m_pUI->progressBarOPMFC2->setValue(int(percentage));

        //if (percentage > 0)
    }
    else if (mfcNumber == 3) {
        // set vertical progress bar
        double range = m_controller.findMFCByNumber(3)->getRange();
        int percentage = 0;
        if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

        // set max
        m_pUI->lblOPMFC3ActualFlow->setText(QString::number(range, 'f', 1));

        // update the progress bar
        m_pUI->progressBarOPMFC3->setValue(int(percentage));
    }
    else if (mfcNumber == 4) {
        // set vertical progress bar
        double range = m_controller.findMFCByNumber(4)->getRange();
        int percentage = 0;
        if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

        m_pUI->lblOPMFC4ActualFlow->setText(QString::number(range, 'f', 1));

        // update the progress bar
        m_pUI->progressBarOPMFC4->setValue(int(percentage));
    }
    else if (mfcNumber == 5) {
        // set vertical progress bar
        double range = m_controller.findMFCByNumber(5)->getRange();
        int percentage = 0;
        if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

        m_pUI->lblOPMFC5ActualFlow->setText(QString::number(range, 'f', 1));

        // update the progress bar
        m_pUI->progressBarOPMFC5->setValue(int(percentage));
    }
    else if (mfcNumber == 6) {
        // set vertical progress bar
        double range = m_controller.findMFCByNumber(6)->getRange();
        int percentage = 0;
        if (range != 0 && actualFlow != 0) percentage = int((actualFlow / range) * 100); // divide by zero protection

        m_pUI->lblOPMFC6ActualFlow->setText(QString::number(range, 'f', 1));

        // update the progress bar
        m_pUI->progressBarOPMFC6->setValue(int(percentage));
    }
}

void OperatorTab::stageStatusUpdate(QString statusNow, QString statusNext)
{
    m_pUI->texteditOPTabAxisStatus->setText(statusNow);
}

// false = down
// true = up
void OperatorTab::pinsStateChanged(bool state)
{
    if (state) {
        m_pUI->btnOPPinsUpDown->setText("Load Pins Up");
        m_pUI->lblOPPinsState->setText("Down");
    }
    else {
        m_pUI->btnOPPinsUpDown->setText("Load Pins Down");
        m_pUI->lblOPPinsState->setText("Up");
    }
}

void OperatorTab::vacStateChanged(bool state)
{
    if (state) {
        m_pUI->btnOPChuckVacOnOff->setText("Chuck Vacuum Off");
        m_pUI->lblOPChuckVacState->setText("On");
    }
    else {
        m_pUI->btnOPChuckVacOnOff->setText("Chuck Vacuum On");
        m_pUI->lblOPChuckVacState->setText("Off");
    }
}

// home state machine
void OperatorTab::HSM_Startup()
{
    m_pUI->btnOPHome->setText("Stop");

    m_pUI->btnOPInit->setEnabled(false);
    m_pUI->btnOPPinsUpDown->setEnabled(false);
    m_pUI->btnOPChuckVacOnOff->setEnabled(false);
    m_pUI->btnOPStartPlasma->setEnabled(false);
}

void OperatorTab::HSM_Done()
{
    m_pUI->btnOPHome->setText("Home");

    m_pUI->btnOPHome->setChecked(false);
    m_pUI->btnOPInit->setEnabled(true);
    m_pUI->btnOPPinsUpDown->setEnabled(true);
    m_pUI->btnOPChuckVacOnOff->setEnabled(true);
    m_pUI->btnOPStartPlasma->setEnabled(true);
    m_pUI->btnOPPinsUpDown->setText("Load Pins Up");
    m_pUI->lblOPPinsState->setText("Down");
}

// init state machine
void OperatorTab::ISM_Startup()
{
    m_pUI->btnOPInit->setEnabled(false);
    m_pUI->btnOPPinsUpDown->setEnabled(false);
    m_pUI->btnOPChuckVacOnOff->setEnabled(false);
    m_pUI->btnOPStartPlasma->setEnabled(false);
    m_pUI->btnOPHome->setEnabled(false);
}

void OperatorTab::ISM_Done()
{
    m_pUI->btnOPInit->setEnabled(true);
    m_pUI->btnOPPinsUpDown->setEnabled(true);
    m_pUI->btnOPChuckVacOnOff->setEnabled(true);
    m_pUI->btnOPStartPlasma->setEnabled(true);
    m_pUI->btnOPHome->setEnabled(true);
    m_pUI->btnOPPinsUpDown->setText("Load Pins Down");
    m_pUI->lblOPPinsState->setText("Up");
}

// collision state machine
void OperatorTab::CSM_StatusUpdate(QString status, QString next)
{
    m_pUI->texteditOPTabAxisStatus->setText(status);
}

// scan state machine
void OperatorTab::SSM_Started()
{
    m_pUI->init_button->setEnabled(false);

    m_pUI->btnOPInit->setEnabled(false);
    m_pUI->btnOPHome->setEnabled(false);
    m_pUI->btnOPPinsUpDown->setEnabled(false);
    m_pUI->btnOPChuckVacOnOff->setEnabled(false);
}

void OperatorTab::SSM_Done()
{
    // update status
    m_pUI->texteditOPTabAxisStatus->setText("Scanning Completed");

    m_pUI->btnOPInit->setEnabled(true);
    m_pUI->btnOPHome->setEnabled(true);
    m_pUI->btnOPPinsUpDown->setEnabled(true);
    m_pUI->btnOPChuckVacOnOff->setEnabled(true);
}

void OperatorTab::SSM_StatusUpdate(QString status, QString next)
{
    m_pUI->axisstatus->setText(status);
}

void OperatorTab::forwardWattsChanged()
{
    int watts = m_controller.getPower().getForwardWatts();

    m_pUI->lblOPRFPowerForward->setText(QString::number(watts));
}

void OperatorTab::MBactualPositionChanged(const double actualPosition)
{
    QString sDisplayValue = QString::number(actualPosition, 'f', 2);

    m_pUI->lblOPMBPosition->setText(sDisplayValue);
}

void OperatorTab::headTemperatureChanged()
{
    double temp = m_controller.getPlasmaHead().getTemperature();

    m_pUI->lblOPHeadTemp->setText(QString::number(temp, 'f', 1));
}

void OperatorTab::plasmaStateChanged(bool plasmaActive)
{
    if (plasmaActive) {
        m_pUI->btnOPStartPlasma->setText("Plasma Off");
        m_pUI->frameOPPlasmaPlume->setStyleSheet("background-color: rgb(246, 77, 241);");
    }
    else {
        m_pUI->btnOPStartPlasma->setChecked(false);
        m_pUI->btnOPStartPlasma->setText("Start Plasma");
        m_pUI->frameOPPlasmaPlume->setStyleSheet("background-color: rgb(52, 101, 164)");
    }
}

void OperatorTab::thicknessChanged()
{
    m_pUI->lblOPThickness->setText(m_controller.getRecipe()->getThicknessQStr());
}

void OperatorTab::gapChanged()
{
    m_pUI->lblOPGap->setText(m_controller.getRecipe()->getGapQStr());
}


//////////////////////// Buttons ////////////////////
void OperatorTab::btnChuckVacOnOff_clicked(bool checked)
{
    if (checked) {
        m_controller.getAxesController().toggleVacOn();
    }
    else {
        m_controller.getAxesController().toggleVacOff();
    }
}


void OperatorTab::btnOPPinsUpDown_clicked(bool checked)
{
    if (checked) {
        m_controller.getAxesController().togglePinsOff(); // pins down
    }
    else {
        m_controller.getAxesController().togglePinsOn(); // pins up
    }
}


void OperatorTab::btnOPStartPlasma_clicked(bool checked)
{
    if (!checked) {
        m_controller.StopScan();
        m_controller.RunRecipe(false); // turn off recipe execution
        m_pUI->btnOPStartPlasma->setText("Start Plasma");
    }
    else {
        m_pUI->btnOPStartPlasma->setText("Plasma Off");

        if (m_controller.getCollision() && m_controller.getRecipe()->getAutoScanBool() && !m_controller.getPlasmaActive()) {
            m_controller.plannedAutoStartOn();//this will make sure we dont accidently start plasma when just clicking RUN SCAN button
            m_controller.StartScan();
        }
        else {
            m_controller.RunRecipe(true); // turn on recipe execution
        }
    }
}

void OperatorTab::btnOPHome_clicked(bool checked)
{
    if (checked) {
        m_controller.getAxesController().StartHome();
    }
    else {
        m_pUI->btnOPHome->setChecked(false);
        m_controller.getAxesController().StopHome();
    }
}


void OperatorTab::btnOPInit_clicked()
{
    m_controller.getAxesController().StartInit();
}


void OperatorTab::comboBoxOPRecipe_currentTextChanged(const QString &arg1)
{
    QString fullPathToRecipeFile = RECIPE_DIRECTORY;
    fullPathToRecipeFile += "/";
    fullPathToRecipeFile += arg1;

    this->m_pMWndw->OpenRecipeFileSelected(fullPathToRecipeFile);
}

void OperatorTab::comboBoxOPLogin_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Engineer")
        this->m_pMWndw->engineerModeSelected();
}





