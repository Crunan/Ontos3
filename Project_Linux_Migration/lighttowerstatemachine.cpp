#include "lighttowerstatemachine.h"

LightTowerStateMachine::LightTowerStateMachine()
    : currentState(INACTIVE)
{
}

LightTowerStateMachine::LightState LightTowerStateMachine::getState() const
{
    return currentState;
}

void LightTowerStateMachine::setState(LightState newState)
{
    if (currentState != newState) {
        currentState = newState;
        emit stateChanged();
    }
}
void LightTowerStateMachine::setLightTower() {
    if ( ((GlobalmyStatusBits && 0x80) > 0) || (doorsOpen) ) {
        LightTower.state = ERROR;
    }
    else if ( (!doorsOpen) && (RunRecipeOn) ) {
        LightTower.state = ACTIVE;
    }
    else {
        if (LightTower.state != READY) {
            LightTower.state = READY;
        }
    }

    switch (LightTower.state) {
    case ERROR:
        writeRequest("$CB01%", 6); //$CB0n% resp[!CB0n#] n = 0,1,2,3 (none, red, amber, green)
        readData();
        LightTower.state = INACTIVE;
        break;
    case READY:
        writeRequest("$CB02%", 6);
        readData();
        LightTower.state = INACTIVE;
        break;
    case ACTIVE:
        writeRequest("$CB03%", 6);
        readData();
        LightTower.state = INACTIVE;
        break;
    case INACTIVE:
        //Do nothing
        break;

    }
}
