#include "lighttower.h"

LightTower::LightTower()
    : currentState(INACTIVE)
{
}

LightTower::LightState LightTower::getState() const
{
    return currentState;
}

void LightTower::setState(LightState newState)
{
    currentState = newState;
    emit lightTowerStateChanged();
}

