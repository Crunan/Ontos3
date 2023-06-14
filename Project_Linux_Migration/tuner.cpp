#include "tuner.h"

Tuner::Tuner()
    : currentPosition(0.0), loadedSetpoint(0.0), autoTune(true)
{
}

double Tuner::getPosition() const
{
    return currentPosition;
}

void Tuner::setPosition(double newPosition)
{
    if (currentPosition != newPosition)
    {
        currentPosition = newPosition;
        emit positionChanged();
    }
}
