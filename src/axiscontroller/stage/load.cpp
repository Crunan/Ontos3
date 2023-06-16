#include "include/axiscontroller/stage/load.h"

Load::Load()
{

}


//void MainWindow::RunHomeAxesSM() {

//    switch (HomeSM.getState()) {
//    case HASM_STARTUP:
//        InitSM.setState(IASM_IDLE);
//        TwoSpotSM.setState(TSSM_IDLE);
//        ScanSM.setState(SASM_IDLE);
//        ScanSM.setSubState(SSSM_IDLE);
//        setHomeBtnText("STOP");
//        updateStageStatus("Homing Startup - Parking Z", "Homing X & Y");
//        move("2", Param.getZMaxSpeedQStr(), stage.getPinsBuriedPosQStr()); //add function
//        logInfo("Homing SM Start Up");
//        HomeSM.setState(HASM_WAIT_PARK_Z);
//        break;
//    case HASM_WAIT_PARK_Z:
//        if (nextStateReady()) {
//            logInfo("Homing Z Parked");
//            HomeSM.setState(HASM_HOME_XY);
//        }
//        break;
//    case HASM_HOME_XY:
//        updateStageStatus("Homing X & Y", "Homing Z");
//        move("0", Param.getXMaxSpeedQStr(), Param.getXHomePosQStr());
//        move("1", Param.getYMaxSpeedQStr(), Param.getYHomePosQStr());
//        logInfo("Homing X & Y");
//        HomeSM.setState(HASM_WAIT_HOME_XY);
//        break;
//    case HASM_WAIT_HOME_XY:
//        if (nextStateReady()) {
//            HomeSM.setState(HASM_HOME_Z);
//            logInfo("Homing Z Parked");
//        }
//        break;
//    case HASM_HOME_Z:
//        updateStageStatus("Homing Z", "");
//        logInfo("Homing Z Parked");
//        move("2", Param.getZMaxSpeedQStr(), Param.getZHomePosQStr());
//        HomeSM.setState(HASM_WAIT_HOME_Z);
//        break;
//    case HASM_WAIT_HOME_Z:
//        if (nextStateReady()) {
//            updateStageStatus("", "");
//            setHomeBtnText("LOAD");
//            logInfo("Z Homed");
//            HomeSM.setState(HASM_WAIT_HOME_Z);
//        }
//        break;
//    case HASM_SHUTDOWN:
//        stopMotors();
//        updateStageStatus("Operator Abort Homing", "");
//        setHomeBtnText("LOAD");
//        logInfo("Operator Abort Homing");
//        move("2", Param.getZMaxSpeedQStr(), Param.getZHomePosQStr());
//        updateStageStatus("", "");
//        HomeSM.setState(HASM_IDLE);
//        break;
//    case HASM_IDLE:
//        //DO NOTHING
//        break;
//    }
//}
