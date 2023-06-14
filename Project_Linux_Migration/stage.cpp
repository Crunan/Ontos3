#include "stage.h"

Stage::Stage()
{
}

double Stage::getStageBase() const
{
    return stageBase.getPosition();
}

void Stage::setStageBase(double newPos)
{
    stageBase.setPosition(newPos);
}

double Stage::getPinsBuried() const
{
    return pinsBuried.getPosition();
}

void Stage::setPinsBuried(double newPos)
{
    pinsBuried.setPosition(newPos);
}

double Stage::getPinsExposed() const
{
    return pinsExposed.getPosition();
}

void Stage::setPinsExposed(double newPos)
{
    pinsExposed.setPosition(newPos);
}
