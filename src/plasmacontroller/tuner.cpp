#include "include/plasmacontroller/tuner.h"

Tuner::Tuner()
    : currentPosition_(0.0), loadedSetpoint_(0.0), autoTune_(false)
{
}

double Tuner::getCurrentPosition() const
{
    return currentPosition_;
}

void Tuner::setCurrentPosition(double position)
{
    if (currentPosition_ != position)
    {
        currentPosition_ = position;
        emit positionChanged();
    }
}

double Tuner::getLoadedSetpoint() const
{
    return loadedSetpoint_;
}

void Tuner::setLoadedSetpoint(double setpoint)
{
    if (loadedSetpoint_ != setpoint)
    {
        loadedSetpoint_ = setpoint;
        emit setpointChanged();
    }
}

bool Tuner::getAutoTune() const
{
    return autoTune_;
}

void Tuner::setAutoTune(bool state)
{
    if (autoTune_ != state)
    {
        autoTune_ = state;
        emit autoTuneChanged();
    }
}

void Tuner::toggleAutoTune()
{
    autoTune_ = !autoTune_;
    emit autoTuneChanged();
}
