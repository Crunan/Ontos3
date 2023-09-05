#include "statemachine.h"

StateMachine::StateMachine(QObject* parent)
    : QObject(parent), currentState(IDLE)
{

}

StateMachine::~StateMachine()
{

}

StateMachine::StageState StateMachine::getState() const
{
    return currentState;
}

void StateMachine::setState(StageState state)
{
    if (currentState != state)
    {
        currentState = state;
        emit stateChanged();
    }
}
