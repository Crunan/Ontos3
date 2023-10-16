#include "axis.h"
#include "logger.h"

Axis::Axis(QObject* parent)
    : QObject(parent),
    m_currentState(0),
    m_currentError(0),
    m_currentPosition(0),
    m_maxPos(0),
    m_maxSpeed(0),
    m_homePos(0),
    m_scanMinBase(0),
    m_scanMaxBase(0)
{
}

Axis::~Axis()
{
    // Destructor
}

void Axis::setCurrentState(const QString state)
{
    bool ok;
    m_currentState = state.toInt(&ok, 16);

    if (!ok) {
        Logger::logCritical("Axis::setCurrentState() invalid hex conversion.  Data = " + state);
    }
}

void Axis::setCurrentError(const QString error)
{
    bool ok;
    m_currentError = error.toInt(&ok, 16);

    if (!ok) {
        Logger::logCritical("Axis::setCurrentError() invalid hex conversion.  Data = " + error);
    }
}

void Axis::setCurrentPosition(const QString position)
{
    bool ok;
    m_currentPosition = position.toDouble(&ok);

    if (!ok) {
        Logger::logCritical("Axis::setCurrentPosition() invalid double conversion.  Data = " + position);
    }
}







