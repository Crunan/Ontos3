#include "pwr.h"

PWR::PWR()
    : loadedSetpoint(0), forwardWatts(0.0), reflectedWatts(0.0), maxWatts(0)
{
}

double PWR::getLoadedSetpoint() const
{
    return loadedSetpoint;
}

void PWR::setLoadedSetpoint(int value)
{
    if (loadedSetpoint != value)
    {
        loadedSetpoint = value;
        emit loadedSetpointChanged();
    }
}
double PWR::getForwardWatts() const
{
    return forwardWatts;
}

double PWR::getReflectedWatts() const
{
    return reflectedWatts;
}
