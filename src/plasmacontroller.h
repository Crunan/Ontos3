#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "axescontroller.h"
#include "commandmap.h"
#include "configuration.h"
#include "ledstatus.h"
#include "plasmahead.h"
#include "pwr.h"
#include "tuner.h"
#include "mfc.h"
#include "settingsdialog.h"

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
    MFC* findMFCByNumber(int mfcNumber);
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

public:
    PlasmaHead m_plasmaHead;
    PWR m_pwr;
    Tuner m_tuner;
    QList<MFC*> m_mfcs; // Store the MFCs in a list

private:
    QSerialPort serialPort_;  // Reference to SerialComm object
    AxesController* axesCTL;  // Optional, can be nullptr
    LEDStatus ledStatus;
    bool executeRecipe;
    Configuration config;
    CommandMap commandMap;

};

#endif // PLASMACONTROLLER_H
