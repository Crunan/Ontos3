#include "axis.h"
#include "coordinate.h"

Axis::Axis(QObject* parent)
    : QObject(parent), currentState(AxisState::IDLE), error(0)
{
    // Initialize the position coordinate
    position.setPosition(0.0);
}

Axis::~Axis()
{
    // Destructor
}

AxisState Axis::getCurrentState() const
{
    return currentState;
}

void Axis::setCurrentState(AxisState state)
{
    if (currentState != state) {
        currentState = state;
        emit currentStateChanged();
    }
}

int Axis::getError() const
{
    return error;
}

double Axis::getPosition() const
{
    return position.getPosition(); // Calling getPosition() from the Coordinate instance
}


