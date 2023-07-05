#include "include/plasmacontroller/pwr.h"

PWR::PWR()
    : recipeWatts_(0),
    forwardWatts_(0),
    reflectedWatts_(0),
    defaultRecipe_(0),
    maxWatts_(0)
{
}

int PWR::getRecipeWatts() const
{
    return recipeWatts_;
}

void PWR::setRecipeWatts(int value)
{
    if (recipeWatts_ != value) {
        recipeWatts_ = value;
        emit recipeWattsChanged();
    }
}

int PWR::getForwardWatts() const
{
    return forwardWatts_;
}


void PWR::setForwardWatts(int value)
{
    if (forwardWatts_ != value) {
        forwardWatts_ = value;
        emit forwardWattsChanged();
    }
}

int PWR::getReflectedWatts() const
{
    return reflectedWatts_;
}
void PWR::setReflectedWatts(int value)
{
    if (reflectedWatts_ != value) {
        reflectedWatts_ = value;
        emit reflectedWattsChanged();
    }
}
int PWR::getDefaultRecipe() const
{
    return defaultRecipe_;
}

void PWR::setDefaultRecipe(int value)
{
    if (defaultRecipe_ != value) {
        defaultRecipe_ = value;
        emit defaultRecipeChanged();
    }
}
