#include "statemachine.h"
#include "functions.h"

StateMachine::StateMachine()
{

}

//void StateMachine() {
//    switch(SM_State) {
//        case STARTUP:
//            if (CTLResetTimeOut > 0) {
//                CTLResetTimeOut -= 1;
//            }
//            else {
//                WriteLogLine("Main State Machine Start Up");
//                //RunStartUp();
//                SM_State = POLLING;
//                UpdateStatus(0);
//            }
//        case POLLING:
//            RunCheckInput();

//            SM_PollCounter += 1; //'increment every main tick loop (100 msec period)
//            if (SM_PollCounter >= SM_POLL_PERIOD) {
//                SM_PollCounter = 0;
//                RunPolling();

//                RunInitAxesSM();
//                RunTwoSpotSM();
//                RunScanSM();
//                RunHomeAxesSM();

//                RunCollisionPassSM();
//                CollisionLaser();
//                SetLightTower();
//                //UpdateStatus(StatusBits);
//            }
//        case SHUTDOWN:
//            WriteLogLine("Main State Machine Shut Down");
//            RunShutDown();
//            SM_State = IDLE;
//            UpdateStatus(0);

//        case IDLE: ;
//    }
//}
