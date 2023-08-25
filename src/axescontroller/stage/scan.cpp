#include "include/axescontroller/stage/scan.h"

Scan::Scan()
{

}

//void MainWindow::RunScanAxesSM() {
//    QString message;

//    switch (ScanSM.getState()) {
//    case SASM_STARTUP:
//        setScanBtnText("STOP");

//        ScanSM.determineSubstrateArea();
//        ScanSM.determineStageZMinMax();
//        ScanSM.determineScanSweep();
//        ScanSM.calculateTotalScanXY();
//        ScanSM.calculateNumScanSweeps();
//        ScanSM.setCurrentRow(1);
//        ScanSM.setCurrentCycle(1);
//        ScanSM.calculateStartingPositions();
//        ScanSM.setRecipeScanSpeed();
//        logStageScanInfo();

//        ScanSM.setState(SASM_SCAN);
//        ScanSM.setSubState(SSSM_PARK_Z);
//        updateStageStatus("Scanning", "");
//        break;
//    case SASM_SCAN:
//        if (nextStateReady()) {

//            switch (ScanSM.getSubState()) {
//            case SSSM_PARK_Z:
//                if (ScanSM.finishedScanning()) {
//                    ScanSM.setSubState(SSSM_IDLE); //for completeness
//                    ScanSM.setState(SASM_RECYCLE); //All done!
//                    message = "End Cycle #" + ScanSM.getCurrentCycleString() + " of " + ScanSM.getScanCyclesString();
//                    logInfo(message);
//                    updateStageStatus("Scanning", message);
//                    setScanBtnText("STOP");
//                }
//                else {
//                    ScanSM.setSubState(SSSM_GO_XY_START);
//                    message = "Parking Z";
//                    logInfo(message);
//                    updateStageStatus("", message);
//                    setScanBtnText("STOP");
//                }
//                move("2", Param.getZMaxSpeedQStr(), ScanSM.getminZString());
//                break;
//            case SSSM_GO_XY_START:
//                message = "Scan Cycle #" + ScanSM.getCurrentCycleString() + " of " + ScanSM.getScanCyclesString() + " Traversal #" + ScanSM.getCurrentRowString() + " of " + ScanSM.getNumRowsString();
//                logInfo(message);
//                updateStageStatus("", message);
//                ScanSM.setNextSweepStartingPosition();

//                move("0", Param.getXMaxSpeedQStr(), ScanSM.getStartingXPositionString());
//                move("1", Param.getYMaxSpeedQStr(), ScanSM.getStartingYPositionString());
//                ScanSM.setSubState(SSSM_GO_Z_SCAN_POS);
//                break;
//            case SSSM_GO_Z_SCAN_POS:
//                if (recipe.getPurge()) {
//                    writeRequest("$C701%", 6); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
//                    readData();
//                }
//                move("2", Param.getZMaxSpeedQStr(), ScanSM.getmaxZString());
//                ScanSM.setSubState(SSSM_SCAN_COL);
//                break;
//            case SSSM_SCAN_COL:
//                move("1", ScanSM.getScanSpeedString(), ScanSM.getEndingYPositionString());
//                ScanSM.incrementNumRow();
//                ScanSM.setSubState(SSSM_PARK_Z);
//                break;
//            case SSSM_IDLE:
//                //do nothin
//                break;
//            }
//        }
//        break;
//    case SASM_RECYCLE:
//        if (ScanSM.shouldContinueScanning()) {
//            setValve2(0);
//            setScanBtnText("SCAN");
//            updateStageStatus("Scanning Completed", "");
//            ScanSM.setState(SASM_IDLE);
//            HomeSM.setState(HASM_STARTUP);
//        }
//        else {
//            ScanSM.incrementCurrentCycle();
//            ScanSM.setCurrentRow(1);
//            ScanSM.calculateStartingPositions();
//            ScanSM.setState(SASM_SCAN);
//            ScanSM.setSubState(SSSM_GO_XY_START);
//            logStageScanInfo();
//        }
//        break;
//    case SASM_SHUTDOWN:
//        if (ScanSM.getSubState() == SSSM_IDLE) {
//            ScanSM.setState(SASM_IDLE);
//        }
//        else {
//            updateStageStatus("Scanning Stopped - Parking Z", "");
//            stopMotors();
//            setValve2(0);
//            move("2", Param.getZMaxSpeedQStr(), ScanSM.getminZString());
//            ScanSM.setState(SASM_IDLE);
//            ScanSM.setSubState(SSSM_IDLE);
//            updateStageStatus("Scanning Manually Stopped", "");
//            setScanBtnText("SCAN");
//        }
//        break;
//    case SASM_IDLE:
//        //DO NOTHING
//        break;
//    }
//}
