#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H


#include "include/axiscontroller/axiscontroller.h"
#include "include/commandmap.h"
#include "include/configuration.h"
#include "include/ledstatus.h"
#include "include/plasmacontroller/plasmahead.h"
#include "include/plasmacontroller/pwr.h"
#include "include/plasmacontroller/tuner.h"
#include "include/plasmacontroller/mfc.h"
#include "include/serialportmanager.h"

#include <QObject>
#include <vector>
#include <memory>


class PlasmaController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool executeRecipe READ getExecuteRecipe WRITE setExecuteRecipe NOTIFY executeRecipeChanged)

public:
    explicit PlasmaController(QWidget* parent = nullptr);
    ~PlasmaController();

    bool getExecuteRecipe() const;
    void setExecuteRecipe(bool value);

    // CTL Commands
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);
    QString findCommandValue(QString command) const;

    // Serial Functions
    QString formatSerialCommand(QString cmd, const QString& setpoint);
    SerialPortManager* getSerialPortManager();    
    QString sendSerialCommand(const QString& command);
    void getCTLStatusCommand();
    void parseResponseForCTLStatus(const QString &response);

    // CTL Status
    void setLEDStatus(int& bits);

    // MFC public commands
    // MFC functions
    MFC* findMFCByNumber(int &mfcNumber);
    int numberOfMFCs();

signals:
    void executeRecipeChanged();

public slots:
    // Define slots for each command logic

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
    PlasmaHead plasmaHead;
    PWR pwr;
    Tuner tuner;
    QList<MFC*> mfcs; //Store the MFCs in a list
    CommandMap commandMap;
    Configuration config;
    AxisController* axisCTL;  // Optional, can be nullptr
    SerialPortManager serial;  // Reference to SerialComm object
    LEDStatus ledStatus;
    bool executeRecipe;

};

#endif // PLASMACONTROLLER_H
