#include "stagestatemachine.h"

StageStateMachine::StageStateMachine(QObject* parent)
    : QObject(parent), currentState(IDLE)
{
}

StageStateMachine::~StageStateMachine()
{
}

auto StageStateMachine::getState() const
{
    return currentState;
}

void StageStateMachine::setState(StageState state)
{
    if (currentState != state) {
        currentState = state;
        emit stateChanged();
    }
}

void StageStateMachine::move(QString axis, QString speed, QString position)
{    
    setState(MOVING);
    setSpeed(axis, speed);
    setAbsMove(axis, position);
}

void StageStageMachine::moveSequence(const QList<double>& sequence)
{
    movementSequence = sequence;
    // Logic to iterate through the movement sequence and perform the movements
    setState(MOVING);
}

void StageStateMachine::stop()
{
    // Logic to stop the stage movement
    setState(STOPPED);
}

void StageStateMachine::emergencyStop()
{
    // Logic to perform emergency stop
    setState(ERROR);
}
