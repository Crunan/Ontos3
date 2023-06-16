#include "include/plasmacontroller/commands.h"

Commands::Commands()
{

}

void Commands::getBatchIDLogging() {
    writeRequest("$2A011%", 7); //GET BatchIDLogging $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 1);
        if (StringIsValidIntChars(StrVar)) {
            Param.setBatchLogging(StrVar);
            logInfo("Batch Logging On/Off: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve Batch Logging, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMBPosition() {
    writeRequest("$2A606%", 7); //GET RECIPE MB Start Position () $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 7);
        if (StringIsValidDoubleChars(StrVar)) {
            TUNER.setLoadedValue(StrVar);
            setRecipeMB();
            logInfo("Loaded MB Setpoint: " + StrVar + " %");
        }
    }
    else
        logCritical("Could Not retrieve MB tuner setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeRFPosition() {
    writeRequest("$2A605%", 7); //GET RECIPE RF PWR Setpoint (Watts) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 4);
        if (StringIsValidIntChars(StrVar)) {
            RF.setLoadedSetPoint(StrVar);
            setRecipeRF();
            logInfo("Loaded RF Setpoint: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve RF setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC4Flow() {
    writeRequest("$2A604%", 7); //GET RECIPE MFC4 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[4].setLoadedFlow(StrVar);
            MFC4RecipeFlow();
            logInfo("Loaded MFC 4 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 4 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC3Flow() {
    writeRequest("$2A603%", 7); //GET RECIPE MFC3 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[3].setLoadedFlow(StrVar);
            MFC3RecipeFlow();
            logInfo("Loaded MFC 3 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 3 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC2Flow() {
    writeRequest("$2A602%", 7); //GET RECIPE MFC2 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[2].setLoadedFlow(StrVar);
            MFC2RecipeFlow();
            logInfo("Loaded MFC 2 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 2 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getRecipeMFC1Flow() {
    writeRequest("$2A601%", 7); //GET RECIPE MFC1 Flow (SLPM) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, (RCV.length() - 8));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[1].setLoadedFlow(StrVar);
            MFC1RecipeFlow();
            logInfo("Loaded MFC 1 Flow Rate: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 1 setpoint, last requestData sent: " + requestData );
}
void MainWindow::getMFC4Range() {
    writeRequest("$8504%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    readData();
    if (RCV.length() > 6) {
        QString StrVar = RCV.mid(5, (RCV.length() - 6));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[4].setRange(StrVar);
            logInfo("Loaded MFC 4 Range: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 4 range, last requestData sent: " + requestData );
}
void MainWindow::getMFC3Range() {
    writeRequest("$8503%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    readData();
    if (RCV.length() > 6) {
        QString StrVar = RCV.mid(5, (RCV.length() - 6));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[3].setRange(StrVar);
            logInfo("Loaded MFC 3 Range: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 3 range, last requestData sent: " + requestData );
}
void MainWindow::getMFC2Range() {
    writeRequest("$8502%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    readData();
    if (RCV.length() > 6) {
        QString StrVar = RCV.mid(5, (RCV.length() - 6));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[2].setRange(StrVar);
            logInfo("Loaded MFC 2 Range: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 2 range, last requestData sent: " + requestData );
}
void MainWindow::getMFC1Range() {
    writeRequest("$8501%", 6); //GET_MFC_RANGE $850m% 1<=m<=4; resp[!850xxx.yy#]
    readData();
    if (RCV.length() > 6) {
        QString StrVar = RCV.mid(5, (RCV.length() - 6));
        if (StringIsValidDoubleChars(StrVar)) {
            MFC[1].setRange(StrVar);
            logInfo("Loaded MFC 1 Range: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 1 range, last requestData sent: " + requestData );
}
void MainWindow::getMaxRFPowerForward() {
    writeRequest("$2A705%", 7); //Get Max RF power forward  $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7, 3);
        if (StringIsValidIntChars(StrVar)) {
            RF.setMaxWatts(StrVar);
            logInfo("Loaded Max RF Forward: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve MFC 1 range, last requestData sent: " + requestData );
}
void MainWindow::getAutoMan() {
    writeRequest("$89%", 4); //GET_AUTO_MAN   $89%; resp [!890p#] p=1 AutoMode, p=0 ManualMode
    readData();
    if (RCV.length() > 3) {
        QString StrVar = RCV.mid(3, 2);
        if (StringIsValidHexChars(StrVar)) {
            TUNER.setAutoMode(StrVar);
            if (TUNER.getAutoTune()) {
                toggleAutoTuneOn();
            }
            else {
                toggleAutoTuneOff();
            }
            logInfo("Loaded Tuner Auto Setting: " + StrVar);
        }
    }
    else
        logCritical("Could Not retrieve Auto/Manual setting, last requestData sent: " + requestData );
}
void MainWindow::getTemp() {
    writeRequest("$8C%", 4);
    readData();
    if (RCV.length() > 3) {
        QString StrVar = RCV.mid(3, 4);
        if (StringIsValidDoubleChars(StrVar)) {
            plasmahead.setHeadTemp(StrVar);
            getHeadTemp();
            logInfo("Loaded current temperature: " + StrVar);
        }
        else
            logCritical("Could Not retrieve temperature, last requestData sent: " + requestData );
    }
}

void MainWindow::setHandshakeStatus() {
    bool ok;
    if (StringIsValidHexChars(CTLparametersAndValues[10])) {
        HandshakeStatusBits = CTLparametersAndValues[10].toInt(&ok, 16);
    };
}

void MainWindow::UpdateHandshakeStatus(int myHandshakeStatusBits) {
    if (myHandshakeStatusBits != HandshakeStatusBitsWas) {
        logInfo("Handshake Status Bits Change from " + BinInt2String(HandshakeStatusBitsWas) + " to " + BinInt2String(myHandshakeStatusBits));

        HandshakeStatusBitsWas = myHandshakeStatusBits;

        if ((myHandshakeStatusBits & 0x0001) > 0)
            ui->actionP_RDY->toggle();
        if ((myHandshakeStatusBits & 0x0002) > 0)
            ui->actionP_AVAIL->toggle();
        if ((myHandshakeStatusBits & 0x0004) > 0)
            ui->actionRUN_P->toggle();
    }
}

void MainWindow::getCTLStatus() {
    writeRequest("$91%", 4); //GET_STATUS    $91% ; resp[!91LLRR#] LL = left LEDS, RR = right LEDS
    readData();
}
//CTL Commands
void MainWindow::resetCTL() {
    writeRequest("$90%", 4);
    readData();
}

void MainWindow::turnOnExecRecipe() {
    writeRequest("$8701%", 6); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
    readData();
    ui->plsmaBtn->setText("PLASMA OFF");
    logInfo("Execute Recipe : Enabled");
}
void MainWindow::turnOffExecRecipe() {
    writeRequest("$8700%", 6); //SET_EXEC_RECIPE  $870p% p=1 Execute Recipe, p=0 RF off, Recipe off
    readData();
    ui->plsmaBtn->setText("START PLASMA");
    logInfo("Execute Recipe : Disabled");
}

void MainWindow::howManyMFCs() {
    writeRequest("$2A002%", 7);
    readData();
    if (RCV.length() > 7) {
        QString StrVar = RCV.mid(7,1); //GET Number of MFCs (1-4) $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
        if (StringIsValidIntChars(StrVar)) {
            Param.setMFCs(StrVar);
            logInfo("Number of MFC's: " + StrVar + "");
        }
    }
    else
        logCritical("Could Not set # of MFCs, last requestData: " + requestData );

}

void MainWindow::on_MB_Left_Button_clicked() {
    QString MBLeftSpeed = "$110000" + ui->stepSizeBox->currentText() + "%";
    writeRequest(MBLeftSpeed, MBLeftSpeed.length()); //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    readData();
}

void MainWindow::on_MB_Right_Button_clicked() {
    QString MBRightSpeed = "$110100" + ui->stepSizeBox->currentText() + "%";
    writeRequest(MBRightSpeed, MBRightSpeed.length()); //$110dxxxx%  d=1,0 xxxx = num steps; resp[!110dxxxx#] when move STARTED
    readData();
}

void MainWindow::toggleBatchIDOn() {
    writeRequest("$28011;1%", 9);
    readData();
    logInfo("Batch ID logging : enabled");
}
void MainWindow::toggleBatchIDOff() {
    writeRequest("$28011;0%", 9);
    readData();
    logInfo("Batch ID logging : disabled");
}
