#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "include/axescontroller/axescontroller.h"
#include "include/commandmap.h"
#include "include/configuration.h"
#include "include/ledstatus.h"
#include "include/plasmacontroller/plasmahead.h"
#include "include/plasmacontroller/pwr.h"
#include "include/plasmacontroller/tuner.h"
#include "include/plasmacontroller/mfc.h"
#include "include/settingsdialog.h"

#include <QObject>
#include <vector>
#include <memory>
#include <QSerialPort>

class PlasmaController : public QObject
{
    Q_OBJECT

public:
    explicit PlasmaController(QWidget* parent = nullptr);
    ~PlasmaController();
    // Serial Functions
    bool open(const SettingsDialog& settings);
    void close();
    QString formatSerialCommand(QString cmd, const QString& setpoint);
    bool sendCommand(const QString& command);
    QString readData();

    QString getPortErrorString();
    bool isOpen();

    // Commands Map functions
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);
    QString findCommandValue(QString command) const;


    // Poll Commands
    void getCTLStatusCommand();
    void parseResponseForCTLStatus(const QString &response);

    // CTL Status
    void setLEDStatus(int& bits);

    // MFC public commands
    // MFC functions
    MFC* findMFCByNumber(int &mfcNumber);
    int numberOfMFCs();

    // Recipe functions
    bool getExecuteRecipe() const;
    void setExecuteRecipe(bool value);

    void CTLStartup();

signals:
    void responseReceived(const QString& response);
    void executeRecipeChanged();
    void mainPortOpened();

public slots:
    // MFCs
    int parseResponseForNumberOfMFCs(QString& responseStr);
    double handleGetMFCRecipeFlowCommand(QString& responseStr);

    void handleSetMFCRecipeFlowCommand(const int mfcNumber, const double recipeFlow);
    void handleSetMFCDefaultRecipeCommand(const int mfcNumber, const double recipeFlow);
    void handleSetMFCRangeCommand(const int mfcNumber, const double range);

    // Tuner
    void handleSetTunerDefaultRecipeCommand(const double defaultPosition);
    void handleSetTunerRecipePositionCommand(const double recipePosition);
    void handleSetTunerAutoTuneCommand(const bool value);

    // PWR
    void handleSetPWRDefaultRecipeCommand(const double defaultWatts);
    void handleSetPWRRecipeWattsCommand(const double recipeWatts);
    void handleSetPWRMaxWattsCommand(const double maxWatts);

private:
    QSerialPort serialPort_;  // Reference to SerialComm object
    AxesController* axesCTL;  // Optional, can be nullptr
    LEDStatus ledStatus;
    bool executeRecipe;
    Configuration config;
    CommandMap commandMap;

public:
    PlasmaHead plasmaHead;
    PWR pwr;
    Tuner tuner;
    QList<MFC*> mfcs; // Store the MFCs in a list

};

#endif // PLASMACONTROLLER_H
