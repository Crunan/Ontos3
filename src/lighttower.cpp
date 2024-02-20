#include "lighttower.h"

LightTower::LightTower()
    : currentState(LT_INACTIVE)
{
}

LightTower::LightState LightTower::getState() const
{
    return currentState;
}

void LightTower::setState(LightState newState)
{
    currentState = newState;
    emit lightTowerStateChanged(newState);
}

