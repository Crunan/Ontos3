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

    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);

public slots:

    // Define slots for each command logic
    void handleSetMFC1SetpointCommand(double setpoint);

private:
    PlasmaHead plasmaHead_;
    PWR pwr_;
    Tuner tuner_;
    std::vector<std::unique_ptr<MFC>> mfcList_;
    CommandMap commandMap_;
    Configuration config_;
    AxisController* axisCTL_;  // Optional, can be nullptr
    SerialComms& serialComm_;  // Reference to SerialComm object

};

#endif // PLASMACONTROLLER_H
