#include "plasmahead.h"

PlasmaHead::PlasmaHead() :
    m_safetyGap(0.0),
    m_slitLength(0.0),
    m_slitWidth(0.0),
    m_temperature(0.0)
{
}

void PlasmaHead::setSafetyGap(double safetyGap)
{
    m_safetyGap = safetyGap;
}

double PlasmaHead::getSafetyGap() const
{
    return m_safetyGap;
}

void PlasmaHead::setSlitlength(double slitLength)
{
    m_slitLength = slitLength;
    emit headSlitLengthChanged();
}

double PlasmaHead::getSlitLength() const
{
    return m_slitLength;
}

void PlasmaHead::setSlitWidth(double slitWidth)
{
    m_slitWidth = slitWidth;
    emit headSlitWidthChanged();
}

double PlasmaHead::getSlitWidth() const
{
    return m_slitWidth;
}

double PlasmaHead::getTemperature() const
{
    return m_temperature;
}

void PlasmaHead::setTemperature(double temperature)
{
    m_temperature = temperature;
    emit headTemperatureChanged();
}
