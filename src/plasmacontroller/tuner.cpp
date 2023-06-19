#include "include/plasmacontroller/tuner.h"

Tuner::Tuner()
  : currentPosition(0.0),
    loadedSetpoint(0.0),
    autoTune(false)
{
}

double Tuner::getPosition() const
{
    return currentPosition;
}

void Tuner::setPosition(double position)
{
    if (currentPosition != position)
    {
        currentPosition = position;
        emit positionChanged();
    }
}

double Tuner::getSetpoint() const
{
    return loadedSetpoint;
}

void Tuner::setSetpoint(double setpoint)
{
    if (loadedSetpoint != setpoint)
    {
        loadedSetpoint = setpoint;
        emit setpointChanged();
    }
}

bool Tuner::getAutoTune() const
{
    return autoTune;
}

void Tuner::setAutoTune(bool state)
{
    if (autoTune != state)
    {
        autoTune = state;
        emit autoTuneChanged();
    }
}
void Tuner::toggleAutoTune()
{
    autoTune = !autoTune;
    emit autoTuneChanged();
}
