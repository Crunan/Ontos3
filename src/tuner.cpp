#include "tuner.h"

Tuner::Tuner()
    : actualPosition_(0.0), recipePosition_(0.0), defaultRecipe_(0.0), autoTune_(false)
{
}

double Tuner::getActualPosition() const
{
    return actualPosition_;
}

void Tuner::setActualPosition(double position)
{
    if (actualPosition_ != position)
    {
        actualPosition_ = position;
        emit actualPositionChanged(actualPosition_);
    }
}

double Tuner::getRecipePosition() const
{
    return recipePosition_;
}

void Tuner::setRecipePosition(double setpoint)
{
    if (recipePosition_ != setpoint)
    {
        recipePosition_ = setpoint;
        emit recipePositionChanged(recipePosition_);
    }
}

double Tuner::getDefaultRecipe() const
{
    return defaultRecipe_;
}

void Tuner::setDefaultRecipe(double setpoint)
{
    if (defaultRecipe_ != setpoint)
    {
        defaultRecipe_ = setpoint;
        emit defaultRecipeChanged(defaultRecipe_);
    }
}

bool Tuner::getAutoTune() const
{
    return autoTune_;
}

void Tuner::setAutoTune(bool state)
{
    if (autoTune_ != state)
    {
        autoTune_ = state;
        emit autoTuneChanged(autoTune_);
    }
}

void Tuner::toggleAutoTune()
{
    autoTune_ = !autoTune_;
    emit autoTuneChanged(autoTune_);
}
