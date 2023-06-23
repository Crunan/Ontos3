#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "include/serialcomms.h"
#include "include/plasmacontroller/plasmahead.h"
#include "include/plasmacontroller/pwr.h"
#include "include/plasmacontroller/tuner.h"
#include "include/plasmacontroller/mfc.h"
#include "include/commandmap.h"
#include "include/configuration.h"
#include "include/axiscontroller/axiscontroller.h"
#include "include/serialcomms.h"

#include <QObject>
#include <vector>
#include <memory>

namespace CTL {
class PlasmaController;
}

class PlasmaController : public QObject
{
public:
    explicit PlasmaController(SerialComms& serialComm, QWidget* parent = nullptr);
    ~PlasmaController();

    PlasmaHead plasmaHead;
    PWR pwr;
    Tuner tuner;
    MFC mfc1;
    MFC mfc2;
    MFC mfc3;
    MFC mfc4;
    CommandMap commandMap;
    // CTL Commands
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);
    QString findCommandValue(QString command) const;
    QString prepareCommand(QString cmd, const QString& setpoint);

    // Serial Functions
    void sendSerialCommand(const QString& data);
public slots:

    // Define slots for each command logic
    void handleSetMFC1SetpointCommand();

private:
    Configuration config_;
    AxisController* axisCTL_;  // Optional, can be nullptr
    SerialComms& serialComm_;  // Reference to SerialComm object

};

#endif // PLASMACONTROLLER_H
