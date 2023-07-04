#include "include/plasmacontroller/plasmarecipe.h"

PlasmaRecipe::PlasmaRecipe(QObject* parent) : QObject(parent) {
    // Initialize member variables here
}

PlasmaRecipe::~PlasmaRecipe() {
    // Cleanup any resources here
}


double PlasmaRecipe::getGas1Setpoint() const {
    return gas1Setpoint_;
}

void PlasmaRecipe::setGas1Setpoint(double setpoint) {
    gas1Setpoint_ = setpoint;
    emit gas1SetpointChanged();
}

double PlasmaRecipe::getGas2Setpoint() const {
    return gas2Setpoint_;
}

void PlasmaRecipe::setGas2Setpoint(double setpoint) {
    gas2Setpoint_ = setpoint;
    emit gas2SetpointChanged();
}

double PlasmaRecipe::getGas3Setpoint() const {
    return gas3Setpoint_;
}

void PlasmaRecipe::setGas3Setpoint(double setpoint) {
    gas3Setpoint_ = setpoint;
    emit gas3SetpointChanged();
}

double PlasmaRecipe::getGas4Setpoint() const {
    return gas4Setpoint_;
}

void PlasmaRecipe::setGas4Setpoint(double setpoint) {
    gas4Setpoint_ = setpoint;
    emit gas4SetpointChanged();
}

int PlasmaRecipe::getRfSetpoint() const {
    return RfSetpoint_;
}

void PlasmaRecipe::setRfSetpoint(int setpoint) {
    RfSetpoint_ = setpoint;
    emit RfSetpointChanged();
}

double PlasmaRecipe::getTunerSetpoint() const {
    return tunerSetpoint_;
}

void PlasmaRecipe::setTunerSetpoint(double setpoint) {
    tunerSetpoint_ = setpoint;
    emit tunerSetpointChanged();
}

bool PlasmaRecipe::isAutoTuneOn() const {
    return autoTuneOn_;
}

void PlasmaRecipe::setAutoTuneOn(bool enabled) {
    autoTuneOn_ = enabled;
    emit autoTuneOnChanged();
}
