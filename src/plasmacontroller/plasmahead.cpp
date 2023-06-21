#include "include/plasmacontroller/plasmahead.h"

PlasmaHead::PlasmaHead(double initialSafetyGap, double initialSlitLength, double initialSlitWidth, double initialTemp)
  : safetyGap(initialSafetyGap),
    slitLength(initialSlitLength),
    slitWidth(initialSlitWidth),
    currentTemp(initialTemp)
{
}

double PlasmaHead::getSafetyGap() const
{
    return safetyGap;
}

double PlasmaHead::getSlitLength() const
{
    return slitLength;
}

double PlasmaHead::getSlitWidth() const
{
    return slitWidth;
}

double PlasmaHead::getTemp() const
{
    return currentTemp;
}

void PlasmaHead::setTemp(double temp)
{
    if (currentTemp != temp)
    {
        currentTemp = temp;
        emit tempChanged();
    }
}
