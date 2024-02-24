#include "pwr.h"

PWR::PWR() :
    m_recipeWatts(0),
    m_forwardWatts(0),
    m_reflectedWatts(0),
    m_defaultRecipe(0),
    m_maxWatts(0)
{
}

int PWR::getRecipeWatts() const
{
    return m_recipeWatts;
}

QString PWR::getRecipeWattsQStr() const
{
    return QString::number(m_recipeWatts);
}

void PWR::setRecipeWatts(int value)
{
    m_recipeWatts = value;
    emit recipeWattsChanged(m_recipeWatts);
}

void PWR::updateRecipeWatts(double value)
{
    m_recipeWatts = value;
    emit updateUIRecipeWatts(m_recipeWatts);
}

int PWR::getForwardWatts() const
{
    return m_forwardWatts;
}

void PWR::setForwardWatts(int value)
{
    if (m_forwardWatts != value) {
        m_forwardWatts = value;
        emit forwardWattsChanged(m_forwardWatts);
    }
}

int PWR::getMaxForwardWatts() const
{
    return m_maxWatts;
}
void PWR::setMaxForwardWatts(int value)
{
    m_maxWatts = value;
}

QString PWR::getMaxForwardWattsQStr() const
{
    return QString::number(m_maxWatts);
}

int PWR::getReflectedWatts() const
{
    return m_reflectedWatts;
}
void PWR::setReflectedWatts(int value)
{
    if (m_reflectedWatts != value) {
        m_reflectedWatts = value;
        emit reflectedWattsChanged(m_reflectedWatts);
    }
}
int PWR::getDefaultRecipe() const
{
    return m_defaultRecipe;
}

void PWR::setDefaultRecipe(int value)
{
    m_defaultRecipe = value;
    emit defaultRecipeChanged();
}
