#ifndef PLASMACONTROLLER_H
#define PLASMACONTROLLER_H

#include "include/serialcomms.h"
#include "include/plasmacontroller/plasmahead.h"
#include "include/plasmacontroller/pwr.h"
#include "include/plasmacontroller/tuner.h"
#include "include/plasmacontroller/mfc.h"
#include "include/plasmacontroller/plasmacommands.h"
#include "include/configuration.h"
#include "include/axiscontroller/axiscontroller.h"

#include <QObject>
#include <QList>

namespace CTL {
class PlasmaController;
}

class PlasmaController : public QObject
{
public:
    explicit PlasmaController(SerialComms* port, QWidget* parent = nullptr);
    ~PlasmaController();

private:
    SerialComms* serial;
    PlasmaHead plasmaHead;
    PWR pwr;
    Tuner tuner;
    QList<MFC> mfc;
    PlasmaCommands command;
    Configuration config;
    AxisController* axisCTL;  // Optional, can be nullptr

};

#endif // PLASMACONTROLLER_H
