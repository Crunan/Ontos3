#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "include/serialcomms.h"
#include "include/plasmacontroller/plasmahead.h"
#include "include/plasmacontroller/pwr.h"
#include "include/plasmacontroller/tuner.h"
#include "include/plasmacontroller/mfc.h"
#include "include/configuration.h"
#include "include/commandhandler.h"
#include "include/commandmap.h"
#include "include/axiscontroller/axiscontroller.h"

#include <QObject>
#include <QList>

namespace CTL {
class PlasmaController;
}

class PlasmaController : public QObject
{
public:
    explicit PlasmaController(QWidget* parent = nullptr);
    ~PlasmaController();

public slots:
    // Define slots for each command logic
    void handleSetMFCSetpointCommand(double setpoint);

private:
    PlasmaHead plasmaHead_;
    PWR pwr_;
    Tuner tuner_;
    QList<MFC*> mfc_;
    CommandMap commandMap_;
    Configuration config_;
    AxisController* axisCTL_;  // Optional, can be nullptr

};

#endif // PLASMACONTROLLER_H
