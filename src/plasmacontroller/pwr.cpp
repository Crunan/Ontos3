#include "include/plasmacontroller/pwr.h"

PWR::PWR()
    : loadedSetpoint_(0.0), forwardWatts_(0.0), reflectedWatts_(0.0), maxWatts_(0)
{
}

double PWR::getLoadedSetpoint() const
{
    return loadedSetpoint_;
}

void PWR::setLoadedSetpoint(double value)
{
    if (loadedSetpoint_ != value)
    {
        loadedSetpoint_ = value;
        emit loadedSetpointChanged();
    }
}

double PWR::getForwardWatts() const
{
    return forwardWatts_;
}

double PWR::getReflectedWatts() const
{
    return reflectedWatts_;
}
