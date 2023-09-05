#include "axiscommands.h"

AxisCommands::AxisCommands()
{

}

//QString Commands::reset() {
//    return "$A9%";
//}

//QString Commands::setSpeed(QString axis, QString speed) {
//    QString command = "$B40" + axis + speed + "%";
//    return command;
//}
//QString Commands::setAbsMove(QString axis, QString position) {
//    QString command = "$B60" + axis + position + "%";
//    return command;
//}

//QString Commands::enableJoy() {
//    return "$BE%"; //ENABLE_JOY $BE%; resp [!BE#];
//}
//QString Commands::disableJoy() {
//    return "$BF%"; //DISABLE_JOY $BF%; resp [!BF#];
//}

//void Commands::getXMaxSpeed() {
//    writeRequest("$DA107%", 7); //GET_X_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            Param.setXMaxSpeed(StrVar);
//            logInfo("X Max Speed: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get max speed for X, last requestData: " + requestData );
//}

//void Commands::getYMaxSpeed() {
//    writeRequest("$DA207%", 7); //GET_Y_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            Param.setYMaxSpeed(StrVar);
//            logInfo("Y Max Speed: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get max speed for Y, last requestData: " + requestData );
//}
//void Commands::getZMaxSpeed() {
//    writeRequest("$DA307%", 7); //GET_Z_MAX_SPEED  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            Param.setZMaxSpeed(StrVar);
//            logInfo("Z Max Speed: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get max speed for Z, last requestData: " + requestData );
//}
//void Commands::getPlasmaHeadXCoordinate() {
//    writeRequest("$DA510%", 7); //GET Xp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stageBaseCoordinates.setXp2Base(StrVar);
//            logInfo("Xp to Base: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get X relative to Base, last requestData: " + requestData );
//}
//void Commands::getPlasmaHeadYCoordinate() {
//    writeRequest("$DA520%", 7); //GET Yp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stageBaseCoordinates.setY(StrVar);
//            logInfo("Yp to Base: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get Y relative to Base, last requestData: " + requestData );
//}
//void Commands::getPlasmaHeadZCoordinate() {
//    writeRequest("$DA530%", 7); //GET Zp_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stageBaseCoordinates.setZ(StrVar);
//            logInfo("Zp to Base: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get Z relative to Base, last requestData: " + requestData );
//}
//void Commands::getPlasmaLaserXCoordinate() {
//    writeRequest("$DA511%", 7); //GET Xs_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stageBaseCoordinates.setlaserX2Base(StrVar);
//            logInfo("Xs to Plasma Head: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get X relative to Plasma head, last requestData: " + requestData );
//}
//void Commands::getPlasmaLaserYCoordinate() {
//    writeRequest("$DA521%", 7); //GET Ys_2_PH  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stageBaseCoordinates.setlaserY2Base(StrVar);
//            logInfo("Ys to Plasma Head: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get Y relative to Plasma head, last requestData: " + requestData );
//}
//void Commands::getPHSlitLength() {
//    writeRequest("$DA540%", 7); //GET Plasma Head Slit Length (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            plasmahead.setPlasmaHeadSlitLength(StrVar);
//            logInfo("Plasma Head Slit Length: " + StrVar + " (mm)");
//        }
//    }
//    else
//        logCritical("Could Not get Plasma head slit length, last requestData: " + requestData );
//}
//void Commands::getPHSlitWidth() {
//    writeRequest("$DA541%", 7); //GET Plasma Head Slit Width (mm)  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            plasmahead.setPlasmaHeadSlitWidth(StrVar);
//            logInfo("Plasma Head Slit Width: " + StrVar + " (mm)");
//        }
//    }
//    else
//        logCritical("Could Not get Plasma head slit width, last requestData: " + requestData );
//}
//void Commands::getPHSafetyZGap() {
//    writeRequest("$DA542%", 7); //GET Plasma Head Safety Gap $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            plasmahead.setSafetyGap(StrVar);
//            logInfo("Plasma Head Z Safety Gap: " + StrVar + " (mm)");
//        }
//    }
//    else
//        logCritical("Could Not get Plasma Head Z Safety Gap, last requestData: " + requestData );
//}
//void Commands::getZPinsBuried() {
//    writeRequest("$DA543%", 7); //GET Z Pins Buried Pos (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stage.setPinsBuried(StrVar);
//            logInfo("Z Pins Buried Position: " + StrVar + " (mm)");
//        }
//    }
//    else
//        logCritical("Could Not get Z Pins Buried Position, last requestData: " + requestData );
//}
//void Commands::getZPinsExposed() {
//    writeRequest("$DA544%", 7); //GET Z Pins Exposed Pos (mm) $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            stage.setPinsExposed(StrVar);
//            logInfo("Z Pins Exposed Position: " + StrVar + " (mm)");
//        }
//    }
//    else
//        logCritical("Could Not get Z Pins Exposed Position, last requestData: " + requestData );
//}
//void Commands::getLoadX2Base() {
//    writeRequest("$DA512%", 7); //GET Load_X_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));currentStatusChanged
//        if (StringIsValidDoubleChars(StrVar)) {
//            Param.setXHomePos(StrVar);
//            logInfo("X Load Position: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get X Load position, last requestData: " + requestData );
//}
//void Commands::getLoadY2Base() {
//    writeRequest("$DA522%", 7); //GET Load_Y_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            Param.setYHomePos(StrVar);
//            logInfo("Y Load Position: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get Y Load position, last requestData: " + requestData );
//}
//void Commands::getLoadZ2Base() {
//    writeRequest("$DA532%", 7); //GET Load_Z_2Base  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
//    readData();
//    if (RCV.length() > 7) {
//        QString StrVar = RCV.mid(7, (RCV.length() - 8));
//        if (StringIsValidDoubleChars(StrVar)) {
//            Param.setZHomePos(StrVar);
//            logInfo("Z Load Position: " + StrVar + "");
//        }
//    }
//    else
//        logCritical("Could Not get Z Load position, last requestData: " + requestData );
//}
//void Commands::initX() {
//    writeRequest("$B500%", 6); //start initializing X axis
//    readData();
//}
//void Commands::initY() {
//    writeRequest("$B501%", 6); //start initializing X axis
//    readData();
//}
//void Commands::initZ() {
//    writeRequest("$B502%", 6); //start initializing X axis
//    readData();
//}
//void Commands::setValve2(QString toggle) {
//    writeRequest("$C70" + toggle + "%", 6); //SET_VALVE_2 $C70n% resp[!C70n#] n = 0, 1 (off, on)
//    readData();
//}
//void Commands::stopMotors() {
//    writeRequest("$B3%", 4); //stop all motors
//    readData();
//}

//void Commands::getAxisStatus() {
//    writeRequest("$C0%", 4); //GET_STATUS $C0%; resp[!C0nn;nn;X state;X error;X pos; Y state; Y error, Y pos; Z state; Z error; Z pos#]
//    readData();
//    parseStatus();
//}

//void MainWindow::toggleHeaterOn() {
//    writeRequest("$CE35.0%", 8);
//    readData();
//    logInfo("Heater : enabled");
//}

//void MainWindow::toggleHeaterOff() {
//    writeRequest("$CE00.0%", 8);
//    readData();
//    logInfo("Heater : disabled");
//}
//void MainWindow::toggleVacOn() {
//    writeRequest("$C801%", 6); //SET_VALVE_3 $C80n% resp[!C80n#] n =0, 1 (off, on)
//    readData();
//    logInfo("Vac : enabled");
//}

//void MainWindow::toggleVacOff() {
//    writeRequest("$C800%", 6); //SET_VALVE_3 $C80n% resp[!C80n#] n =0, 1 (off, on)
//    readData();
//    logInfo("Vac : disabled");
//}
//void MainWindow::toggleJoystickOn() {
//    writeRequest("$BE%", 4);
//    readData();
//    logInfo("Joystick : enabled");
//}

//void MainWindow::toggleJoystickOff() {
//    writeRequest("$BF%", 4);
//    readData();
//    logInfo("Joystick : disabled");
//}

//void MainWindow::toggleN2PurgeOn() {
//    recipe.setPurge(true);
//    logInfo("Recipe N2 purge : enabled");
//}
//void MainWindow::toggleN2PurgeOff() {
//    recipe.setPurge(false);
//    logInfo("Recipe N2 purge : disabled");
//}
//void MainWindow::togglePinsOn() {
//    QString StrCmd{};
//    QString pin_pos = QString::number(stage.getPinsExposedPos());
//    StrCmd = "$B602" +  pin_pos + "%";
//    writeRequest(StrCmd, StrCmd.length());
//    readData();
//    logInfo("Stage Pins : enabled");
//}
//void MainWindow::togglePinsOff() {
//    QString StrCmd{};
//    QString pin_pos = QString::number(stage.getPinsBuriedPos());
//    StrCmd = "$B602" +  pin_pos + "%";
//    writeRequest(StrCmd, StrCmd.length());
//    readData();
//    logInfo("Stage Pins : disabled");
//}
