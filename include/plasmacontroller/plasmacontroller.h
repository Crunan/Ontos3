#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "include/serialportmanager.h"
#include "include/plasmacontroller/plasmahead.h"
#include "include/plasmacontroller/pwr.h"
#include "include/plasmacontroller/tuner.h"
#include "include/plasmacontroller/mfc.h"
#include "include/commandmap.h"
#include "include/configuration.h"
#include "include/axiscontroller/axiscontroller.h"

#include <QObject>
#include <vector>
#include <memory>


class PlasmaController : public QObject
{
    Q_OBJECT
public:
    explicit PlasmaController(SerialComms& serialComm, QWidget* parent = nullptr);
    ~PlasmaController();

    PlasmaHead plasmaHead;
    PWR pwr;
    Tuner tuner;
    QList<MFC*> mfcs; //Store the MFCs in a list
    CommandMap commandMap;
    Configuration config;
    AxisController* axisCTL;  // Optional, can be nullptr
    SerialComms& serial;  // Reference to SerialComm object

    // CTL Commands
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);
    QString findCommandValue(QString command) const;

    // Serial Functions
    QString prepareCommand(QString cmd, const QString& setpoint);


public slots:
    // Define slots for each command logic
    // MFCs
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

};

#endif // PLASMACONTROLLER_H
