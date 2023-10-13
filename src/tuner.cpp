#include "tuner.h"

Tuner::Tuner() :
    m_actualPosition(0.0),
    m_recipePosition(0.0),
    m_defaultRecipe(0.0),
    m_autoTune(false)
{
}

double Tuner::getActualPosition() const
{
    return m_actualPosition;
}

void Tuner::setActualPosition(double position)
{
    m_actualPosition = position;
    emit actualPositionChanged(m_actualPosition);
}

double Tuner::getRecipePosition() const
{
    return m_recipePosition;
}

QString Tuner::getRecipePositionQStr() const
{
    return QString::number(m_recipePosition, 'f', 2);
}

void Tuner::setRecipePosition(double setpoint)
{
    m_recipePosition = setpoint;
    emit recipePositionChanged(m_recipePosition);
}

double Tuner::getDefaultRecipe() const
{
    return m_defaultRecipe;
}

void Tuner::setDefaultRecipe(double setpoint)
{
    m_defaultRecipe = setpoint;
    emit defaultRecipeChanged(m_defaultRecipe);
}

bool Tuner::getAutoTune() const
{
    return m_autoTune;
}

void Tuner::setAutoTune(bool state)
{
    m_autoTune = state;
    emit autoTuneChanged(m_autoTune);
}

void Tuner::toggleAutoTune()
{
    m_autoTune = !m_autoTune;
    emit autoTuneChanged(m_autoTune);
}
