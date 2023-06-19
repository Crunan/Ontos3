#include "include/recipe.h"

Recipe::Recipe(QObject* parent) : QObject(parent) {
    // Initialize member variables with default values
    m_substrateThickness = 0.0;
    m_plasmaHeadGap = 0.0;
    m_substrateOverlap = 0.0;
    m_stageSpeed = 0.0;
    m_numCycles = 1;
    m_autoScanOn = false;
    m_N2PurgeOn = false;
    m_gas1Setpoint = 0.0;
    m_gas2Setpoint = 0.0;
    m_gas3Setpoint = 0.0;
    m_gas4Setpoint = 0.0;
    m_RfSetpoint = 0;
    m_tunerSetpoint = 0.0;
    m_autoTuneOn = false;
}

Recipe::~Recipe() {}

double Recipe::getSubstrateThickness() const {
    return m_substrateThickness;
}

void Recipe::setSubstrateThickness(double thickness) {
    if (m_substrateThickness != thickness) {
        m_substrateThickness = thickness;
        emit substrateThicknessChanged();
    }
}

double Recipe::getPlasmaHeadGap() const {
    return m_plasmaHeadGap;
}

void Recipe::setPlasmaHeadGap(double gap) {
    if (m_plasmaHeadGap != gap) {
        m_plasmaHeadGap = gap;
        emit plasmaHeadGapChanged();
    }
}

double Recipe::getSubstrateOverlap() const {
    return m_substrateOverlap;
}

void Recipe::setSubstrateOverlap(double overlap) {
    if (m_substrateOverlap != overlap) {
        m_substrateOverlap = overlap;
        emit substrateOverlapChanged();
    }
}

double Recipe::getStageSpeed() const {
    return m_stageSpeed;
}

void Recipe::setStageSpeed(double speed) {
    if (m_stageSpeed != speed) {
        m_stageSpeed = speed;
        emit stageSpeedChanged();
    }
}

int Recipe::getNumCycles() const {
    return m_numCycles;
}

void Recipe::setNumCycles(int cycles) {
    if (m_numCycles != cycles) {
        m_numCycles = cycles;
        emit numCyclesChanged();
    }
}

bool Recipe::isAutoScanOn() const {
    return m_autoScanOn;
}

void Recipe::setAutoScanOn(bool enabled) {
    if (m_autoScanOn != enabled) {
        m_autoScanOn = enabled;
        emit autoScanOnChanged();
    }
}

bool Recipe::isN2PurgeOn() const {
    return m_N2PurgeOn;
}

void Recipe::setN2PurgeOn(bool enabled) {
    if (m_N2PurgeOn != enabled) {
        m_N2PurgeOn = enabled;
        emit N2PurgeOnChanged();
    }
}

double Recipe::getGas1Setpoint() const {
    return m_gas1Setpoint;
}

void Recipe::setGas1Setpoint(double setpoint) {
    if (m_gas1Setpoint != setpoint) {
        m_gas1Setpoint = setpoint;
        emit gas1SetpointChanged();
    }
}

double Recipe::getGas2Setpoint() const {
    return m_gas2Setpoint;
}

void Recipe::setGas2Setpoint(double setpoint) {
    if (m_gas2Setpoint != setpoint) {
        m_gas2Setpoint = setpoint;
        emit gas2SetpointChanged();
    }
}

double Recipe::getGas3Setpoint() const {
    return m_gas3Setpoint;
}

void Recipe::setGas3Setpoint(double setpoint) {
    if (m_gas3Setpoint != setpoint) {
        m_gas3Setpoint = setpoint;
        emit gas3SetpointChanged();
    }
}

double Recipe::getGas4Setpoint() const {
    return m_gas4Setpoint;
}

void Recipe::setGas4Setpoint(double setpoint) {
    if (m_gas4Setpoint != setpoint) {
        m_gas4Setpoint = setpoint;
        emit gas4SetpointChanged();
    }
}

int Recipe::getRfSetpoint() const {
    return m_RfSetpoint;
}

void Recipe::setRfSetpoint(int setpoint) {
    if (m_RfSetpoint != setpoint) {
        m_RfSetpoint = setpoint;
        emit RfSetpointChanged();
    }
}

double Recipe::getTunerSetpoint() const {
    return m_tunerSetpoint;
}

void Recipe::setTunerSetpoint(double setpoint) {
    if (m_tunerSetpoint != setpoint) {
        m_tunerSetpoint = setpoint;
        emit tunerSetpointChanged();
    }
}

bool Recipe::isAutoTuneOn() const {
    return m_autoTuneOn;
}

void Recipe::setAutoTuneOn(bool enabled) {
    if (m_autoTuneOn != enabled) {
        m_autoTuneOn = enabled;
        emit autoTuneOnChanged();
    }
}
