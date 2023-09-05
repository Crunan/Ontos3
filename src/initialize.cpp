#include "initialize.h"

Initialize::Initialize()
{

}

//void MainWindow::RunInitAxesSM() {
//    switch (InitSM.getState()) {
//    case IASM_STARTUP:
//        //SM set to idle
//        TwoSpotSM.setState(TSSM_IDLE);
//        HomeSM.setState(HASM_IDLE);
//        ScanSM.setState(SASM_IDLE);
//        ScanSM.setSubState(SSSM_IDLE);
//        //send commands
//        stopMotors();
//        setValve2("0");
//        sendInitCMD();
//        //GUI status
//        ui->Stagepins_button->setChecked(true);
//        updateStageStatus("Initializing Axes", "");
//        //log
//        logInfo("Initializing Axes");
//        InitSM.setState(IASM_WAIT_FOR_DONE);
//        break;
//    case IASM_WAIT_FOR_DONE:
//        if (nextStateReady()) {
//            InitSM.setState(IASM_INITIALIZED);
//        }
//        break;
//    case IASM_IDLE:
//        //Do nothing
//        break;
//    case IASM_INITIALIZED:
//        updateStageStatus("Stage Initialized", "");
//        InitSM.setState(IASM_IDLE);
//        break;
//    }
//}
