#include "include/axiscontroller/stage/twospot.h"

TwoSpot::TwoSpot()
{

}


//void MainWindow::RunTwoSpotAxesSM() {

//    switch (TwoSpotSM.getState()) {
//    case TSSM_STARTUP:
//        enableJoy();
//        setTwoSpotBtnText("STOP");
//        updateStageStatus("Joy Stick is Enabled","Spot First Point");
//        logInfo("TwoSpotSM Start Up");
//        logInfo("TwoSpotSM Getting First");
//        TwoSpotSM.setState(TSSM_GET_FIRST);
//        break;
//    case TSSM_GET_FIRST:
//        if (joystickOn) {
//            ui->axisstatus_2->setText("Release JoyStick Button");
//            TwoSpotSM.setFirstX(X.getPositionRelative2Base());
//            TwoSpotSM.setFirstY(Y.getPositionRelative2Base());
//            logInfo("TwoSpotSM Got First");
//            TwoSpotSM.setState(TSSM_WAIT_JOY_BTN_OFF);
//        }
//        break;
//    case TSSM_WAIT_JOY_BTN_OFF:
//        if (!joystickOn) {
//            logInfo("TwoSpotSM Getting Second");
//            updateStageStatus("Spot Second Point", "");
//            TwoSpotSM.setState(TSSM_GET_SECOND);
//        }
//        break;
//    case TSSM_GET_SECOND:
//        if (joystickOn) {
//            TwoSpotSM.setSecondX(X.getPositionRelative2Base());
//            TwoSpotSM.setSecondY(Y.getPositionRelative2Base());
//            //determine box orientation and corners
//            TwoSpotSM.checkXDimensions();
//            TwoSpotSM.checkYDimensions();
//            updateTwoSpotXYText();
//            logInfo("TwoSpotSM Got Second - done");
//            TwoSpotSM.setState(TSSM_SHUTDOWN);
//        }
//        break;
//    case TSSM_SHUTDOWN:
//        disableJoy();
//        TwoSpotSM.setState(TSSM_IDLE);
//        updateStageStatus("", "");
//        setTwoSpotBtnText("TWO SPOT");


//    case TSSM_IDLE:
//        //DO NOTHING
//        break;
//    }
//}
