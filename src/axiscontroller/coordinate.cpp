#include "include/axiscontroller/coordinate.h"


double Coordinate::getPosition() const
{
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
