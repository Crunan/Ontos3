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

void PWR::setRecipeWatts(int value)
{
    m_recipeWatts = value;
    emit recipeWattsChanged(m_recipeWatts);
}

int PWR::getForwardWatts() const
{
    return m_forwardWatts;
}


void PWR::setForwardWatts(int value)
{
    m_forwardWatts = value;
    emit forwardWattsChanged();
}

int PWR::getReflectedWatts() const
{
    return m_reflectedWatts;
}
void PWR::setReflectedWatts(int value)
{
    m_reflectedWatts = value;
    emit reflectedWattsChanged();
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
