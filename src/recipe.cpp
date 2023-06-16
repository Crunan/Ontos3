#include "include/recipe.h"

Recipe::Recipe(QObject* parent) : QObject(parent) {
    // Initialize member variables with default values
    substrateThickness = 0.0;
    plasmaHeadGap = 0.0;
    substrateOverlap = 0.0;
    stageSpeed = 0.0;
    numCycles = 1;
    autoScanOn = false;
    N2PurgeOn = false;
    gas1Setpoint = 0.0;
    gas2Setpoint = 0.0;
    gas3Setpoint = 0.0;
    gas4Setpoint = 0.0;
    RfSetpoint = 0.0;
    tunerSetpoint = 0.0;
    autoTuneOn = false;
}

Recipe::~Recipe() {}

double Recipe::getSubstrateThickness() const {
    return substrateThickness;
}

void Recipe::setSubstrateThickness(double thickness) {
    if (substrateThickness != thickness) {
        substrateThickness = thickness;
        emit substrateThicknessChanged();
    }
}

double Recipe::getPlasmaHeadGap() const {
    return plasmaHeadGap;
}

void Recipe::setPlasmaHeadGap(double gap) {
    if (plasmaHeadGap != gap) {
        plasmaHeadGap = gap;
        emit plasmaHeadGapChanged();
    }
}

double Recipe::getSubstrateOverlap() const {
    return substrateOverlap;
}

void Recipe::setSubstrateOverlap(double overlap) {
    if (substrateOverlap != overlap) {
        substrateOverlap = overlap;
        emit substrateOverlapChanged();
    }
}

double Recipe::getStageSpeed() const {
    return stageSpeed;
}

void Recipe::setStageSpeed(double speed) {
    if (stageSpeed != speed) {
        stageSpeed = speed;
        emit stageSpeedChanged();
    }
}

int Recipe::getNumCycles() const {
    return numCycles;
}

void Recipe::setNumCycles(int cycles) {
    if (numCycles != cycles) {
        numCycles = cycles;
        emit numCyclesChanged();
    }
}

bool Recipe::isAutoScanOn() const {
    return autoScanOn;
}

void Recipe::setAutoScanOn(bool enabled) {
    if (autoScanOn != enabled) {
        autoScanOn = enabled;
        emit autoScanOnChanged();
    }
}

bool Recipe::isN2PurgeOn() const {
    return N2PurgeOn;
}

void Recipe::setN2PurgeOn(bool enabled) {
    if (N2PurgeOn != enabled) {
        N2PurgeOn = enabled;
        emit N2PurgeOnChanged();
    }
}

double Recipe::gas1Setpoint() const {
    return gas1Setpoint;
}

void Recipe::setGas1Setpoint(double setpoint) {
    if (gas1Setpoint != setpoint) {
        gas1Setpoint = setpoint;
        emit gas1SetpointChanged();
    }
}

double Recipe::gas2Setpoint() const {
    return gas2Setpoint;
}

void Recipe::setGas2Setpoint(double setpoint) {
    if (gas2Setpoint != setpoint) {
        gas2Setpoint = setpoint;
        emit gas2SetpointChanged();
    }
}

double Recipe::gas3Setpoint() const {
    return gas3Setpoint;
}

void Recipe::setGas3Setpoint(double setpoint) {
    if (gas3Setpoint != setpoint) {
        gas3Setpoint = setpoint;
        emit gas3SetpointChanged();
    }
}

double Recipe::gas4Setpoint() const {
    return gas4Setpoint;
}

void Recipe::setGas4Setpoint(double setpoint) {
    if (gas4Setpoint != setpoint) {
        gas4Setpoint = setpoint;
        emit gas4SetpointChanged();
    }
}

double Recipe::RfSetpoint() const {
    return RfSetpoint;
}

void Recipe::setRfSetpoint(double setpoint) {
    if (RfSetpoint != setpoint) {
        RfSetpoint = setpoint;
        emit RfSetpointChanged();
    }
}

double Recipe::tunerSetpoint() const {
    return tunerSetpoint;
}

void Recipe::setTunerSetpoint(double setpoint) {
    if (tunerSetpoint != setpoint) {
        tunerSetpoint = setpoint;
        emit tunerSetpointChanged();
    }
}

bool Recipe::isAutoTuneOn() const {
    return autoTuneOn;
}

void Recipe::setAutoTuneOn(bool enabled) {
    if (autoTuneOn != enabled) {
        autoTuneOn = enabled;
        emit autoTuneOnChanged();
    }
}
