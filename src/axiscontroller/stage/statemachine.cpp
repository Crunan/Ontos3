#include "include/axiscontroller/stage/statemachine.h"

Positions::StateMachine()
    :currentState(IDLE)
{
}

Positions::~StateMachine()
{
}

auto StateMachine::getState() const
{
    return currentState;
}

void StateMachine::setState(StageState state)
{
    if (currentState != state) {
        currentState = state;
        emit stateChanged();
    }
}

