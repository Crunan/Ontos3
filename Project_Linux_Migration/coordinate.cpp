#include "coordinate.h"

Coordinate::Coordinate(QObject *parent)
    : QObject(parent), position(0.0)
{

}

double Coordinate::getPosition() const {
    return position;
}

void Coordinate::setPosition(double newPosition)
{
    if (position != newPosition)
    {
        position = newPosition;
        emit positionChanged();
    }
}

