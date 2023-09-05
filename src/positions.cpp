#include "positions.h"

Positions::Positions()
{
}

double Positions::getBase() const {
    return base.getPosition();
}

void Positions::setStageBase(double newPos) {
    base.setPosition(newPos);
}

double Positions::getPinsBuried() const {
    return pinsBuried.getPosition();
}

void Positions::setPinsBuried(double newPos) {
    pinsBuried.setPosition(newPos);
}

double Positions::getPinsExposed() const {
    return pinsExposed.getPosition();
}

void Positions::setPinsExposed(double newPos) {
    pinsExposed.setPosition(newPos);
}
