#include "plasmahead.h"

PlasmaHead::PlasmaHead()
    : safetyGap_(0.0), slitLength_(0.0), slitWidth_(0.0), temperature_(0.0)
{
}

double PlasmaHead::getSafetyGap() const
{
    return safetyGap_;
}

double PlasmaHead::getSlitLength() const
{
    return slitLength_;
}

double PlasmaHead::getSlitWidth() const
{
    return slitWidth_;
}

double PlasmaHead::getTemperature() const
{
    return temperature_;
}

void PlasmaHead::setTemperature(double temperature)
{
    if (temperature != temperature_) {
        temperature_ = temperature;
        emit temperatureChanged();
    }
}
