#include "include/mainstatemachine.h"

MainStateMachine::MainStateMachine(QObject* parent = nullptr)
    : currentState(IDLE) //QStateMachine(parent)
{

}
auto MainStateMachine::getState() const
{
    return currentState;
}

void MainStateMachine::setState(SystemState state)
{
    if (this->currentState != state) {
        this->currentState = state;
        emit stateChanged();
    }
}

//void MainStateMachine::runstateMachine() {
//    switch(MainStateMachine.getState()) {
//    case STARTUP:
//        if (CTLResetTimeOut > 0ms) {
//            CTLResetTimeOut -= 1ms;
//            break;
//        }
//        else {
//            logInfo("Main State Machine Start Up");
//            RunStartup();
//            MainStateMachine.setState(POLLING);
//            UpdateStatus();
//            break;
//        }
//    case POLLING:

//        SM_PollCounter += 1;
//        if (SM_PollCounter >= SM_POLL_PERIOD) {
//            SM_PollCounter = 0;
//            RunPolling();
//            UpdateStatus();
//            setLightTower();
//            RunInitAxesSM();
//            RunTwoSpotAxesSM();
//            RunHomeAxesSM();
//            RunScanAxesSM();

//        }
//        break;
//    case IDLE:
//        break;
//    case SHUTDOWN:
//        break;
//        UpdateStatus(0);
//    }
//}


//void MainWindow::shutDownProgram() {

//}

