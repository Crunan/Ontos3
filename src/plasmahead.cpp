#include "plasmahead.h"

PlasmaHead::PlasmaHead() :
    m_safetyGap(0.0),
    m_slitLength(0.0),
    m_slitWidth(0.0),
    m_temperature(0.0)
{
}

double PlasmaHead::getSafetyGap() const
{
    return m_safetyGap;
}

double PlasmaHead::getSlitLength() const
{
    return m_slitLength;
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
    if (temperature != m_temperature) {
        m_temperature = temperature;
        emit temperatureChanged();
    }
}