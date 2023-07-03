#include "include/plasmacontroller/pwr.h"

PWR::PWR()
    : recipeWatts_(0.0),
    forwardWatts_(0.0),
    reflectedWatts_(0.0),
    defaultRecipe_(0.0),
    maxWatts_(0)
{
}

double PWR::getRecipeWatts() const
{
    return recipeWatts_;
}

void PWR::setRecipeWatts(double value)
{
    if (recipeWatts_ != value) {
        recipeWatts_ = value;
        emit recipeWattsChanged();
    }
}

double PWR::getForwardWatts() const
{
    return forwardWatts_;
}


void PWR::setForwardWatts(double value)
{
    if (forwardWatts_ != value) {
        forwardWatts_ = value;
        emit forwardWattsChanged();
    }
}

double PWR::getReflectedWatts() const
{
    return reflectedWatts_;
}
void PWR::setReflectedWatts(double value)
{
    if (reflectedWatts_ != value) {
        reflectedWatts_ = value;
        emit reflectedWattsChanged();
    }
}
double PWR::getDefaultRecipe() const
{
    return defaultRecipe_;
}

void PWR::setDefaultRecipe(double value)
{
    if (defaultRecipe_ != value) {
        defaultRecipe_ = value;
        emit defaultRecipeChanged();
    }
}
