#ifndef SCANSTATEMACHINE_H
#define SCANSTATEMACHINE_H


//Scan Axes SM
enum ScanState {
    SASM_IDLE,
    SASM_STARTUP,
    SASM_SCAN,
    SASM_RECYCLE,
    SASM_SHUTDOWN
};

struct StageManeuver {

    //Scan Sub Axes SM
//    enum ScannerSubState {
//        SSSM_IDLE,
//        SSSM_PARK_Z,
//        SSSM_GO_XY_START,
//        SSSM_GO_Z_SCAN_POS,
//        SSSM_SCAN_COL
//    };
};

class Scan {

private:
    Scan();

    ScanState currentState;
    StageManeuver currentManeuver;

public:
    void setScanState(ScanState state);
    void performStageManeuver(const StageManeuver& maneuver);


};
#endif // SCANSTATEMACHINE_H

//    ScannerState state;
//    ScannerSubState subState;
//    double minX, maxX, minY, maxY, minZ, maxZ;

//    double m_rowXwidth;
//    double m_colYlength;
//    double m_startXpos;
//    double m_startYpos;
//    double m_endYpos;
//    double m_scanYspeed = 1;
//    int m_numXrows;
//    double m_Xremaining;
//    int m_thisXrow;
//    int m_thisCycleNum;




//    void setZMinPos(double z) {
//        m_minZpos = z;
//    }
//    void setZMaxPos() {
//        bool ok;
//        m_maxZpos = CoordParam.getZp2Base() - recipe.getThickness().toDouble(&ok) - recipe.getGap().toDouble(&ok);
//    }
//    void setXRowWidth(double val1, double val2) {
//        m_rowXwidth = val1 - val2;
//    }
//    void setXTotal() {
//        m_Xremaining = this->getXmax() - this->getXmin();
//    }
//    void setXRemaining() {
//        m_Xremaining = m_Xremaining - this->getRowWidth();
//    }
//    void setThisCycle(int thisCycle) {
//        m_thisCycleNum = thisCycle;
//    }
//    void setThisXRow(int row) {
//        m_thisXrow = row;
//    }
//    void setNumRows(int val) {
//        m_numXrows = val;
//    }
//    void setXNewStartPos(double pos1, double pos2) {
//        m_startXpos = pos1 - pos2;
//    }
//    void setYStartPos() {
//        m_startYpos = this->getYmax() + plasmahead.getPlasmaHeadSlitWidth();
//    }
//    void setYEndPos() {
//        m_endYpos = this->getYmin() + plasmahead.getPlasmaHeadSlitWidth();
//    }
//    void setYSpeed() {
//        bool ok;
//        m_scanYspeed = recipe.getSpeed().toDouble(&ok);
//    }
//    void incrementNumRows() {
//        this->m_numXrows = this->getNumXrows() + 1;
//    }
//    void incrementThisRow() {
//        this->m_thisXrow = this->getThisXRow() + 1;
//    }
//    void incrementCycles() {
//        m_thisCycleNum += 1;
//    }


//    void setXSinglePass() {
//        m_startXpos = (this->getXmax() + this->getXmin()) / 2;
//    }
//    void setXMutltiplePass() {
//        m_startXpos = this->getXmax() - (this->getRowWidth() / 2);
//    }
//    void setSingleOrMultipleScan() {
//        if (this->getNumXrows() == 1) {
//            this->setXSinglePass();
//        }
//        else {
//            this->setXMutltiplePass();
//        }
//    }

//    ScannerState getState() {
//        return m_state;
//    }
//    ScannerSubState getSubState() {
//        return m_subState;
//    }
//    QString getXminQStr() {
//        return QString::number(m_minXpos);
//    }
//    QString getXmaxQStr() {
//        return QString::number(m_maxXpos);
//    }
//    QString getYminQStr() {
//        return QString::number(m_minYpos);
//    }
//    QString getYmaxQStr() {
//        return QString::number(m_maxYpos);
//    }
//    QString getXRemainingQStr() {
//        return QString::number(m_Xremaining);
//    }
//    QString getXNumRowsQStr() {
//        return QString::number(m_numXrows);
//    }
//    QString getXRowWidthQStr() {
//        return QString::number(m_rowXwidth);
//    }
//    QString getXThisRowQStr() {
//        return QString::number(m_thisXrow);
//    }
//    QString getXStartPosQStr() {
//        return QString::number(m_startXpos);
//    }
//    QString getYStartPosQStr() {
//        return QString::number(m_startYpos);
//    }
//    QString getYEndPosQStr() {
//        return QString::number(m_endYpos);
//    }
//    QString getScanYSpeedQStr() {
//        return QString::number(m_scanYspeed);
//    }
//    QString getThisCyclesQStr() {
//        return QString::number(m_thisCycleNum);
//    }
//    QString getZminQStr() {
//        return QString::number(m_minZpos);
//    }
//    QString getZmaxQStr() {
//        return QString::number(m_maxZpos);
//    }
//    double getXStartPos() {
//        return m_startXpos;
//    }
//    double getXRowWidth() {
//        return m_rowXwidth;
//    }
//    int getThisCycle() {
//        return m_thisCycleNum;
//    }

//    double getXmin() {
//        return m_minXpos;
//    }
//    double getXmax() {
//        return m_maxXpos;
//    }
//    double getYmin() {
//        return m_minYpos;
//    }
//    double getYmax() {
//        return m_maxYpos;
//    }
//    double getMinPerPH() {
//        return m_minPerPH;
//    }
//    double getMaxPerPH() {
//        return m_maxPerPH;
//    }
//    double getXremaining() {
//        return m_Xremaining;
//    }
//    double getNumXrows() {
//        return m_numXrows;
//    }
//    double getRowWidth() {
//        return m_rowXwidth;
//    }
//    int getThisXRow() {
//        return m_thisXrow;
//    }
//    bool finishedScanning() {
//        if (this->getThisXRow() > this->getNumXrows()) {
//            return true;
//        }
//        else {
//            return false;
//        }
//    }
//    bool keepScanning() {
//        if (this->getThisCycle() >= recipe.getCyclesInt()) {
//            return true;
//        }
//        else {
//            return false;
//        }
//    }
//    struct StageScanner {
//        ScannerState state = SASM_IDLE;
//        ScannerSubState subState = SSSM_IDLE;
//        int numRows = 0;
//        int currentRow = 0;
//        int currentCycle = 0;
//        int scanCycles = 0;
//        double X1 = 0.0;
//        double X2 = 0.0;
//        double Y1 = 0.0;
//        double Y2 = 0.0;
//        double Z1 = 0.0;
//        double Z2 = 0.0;
//        double minX = 0.0;
//        double maxX = 0.0;
//        double minY = 0.0;
//        double maxY = 0.0;
//        double minZ = 0.0;
//        double maxZ = 0.0;
//        double totalXScan = 0.0;
//        double totalYScan = 0.0;
//        double startingXPosition = 0.0;
//        double startingYPosition = 0.0;
//        double nextSweepStartingPosition = 0.0;
//        double endingYPosition = 0.0;
//        double remainingX = 0.0;
//        double scanSweep = 0.0;
//        double scanSpeed = 0.0;


//        ScannerState getState() const { return state; }
//        void setState(ScannerState value) { state = value; }

//        ScannerSubState getSubState() const { return subState; }
//        void setSubState(ScannerSubState value) { subState = value; }

//        QString getX1String() const { return QString::number(X1); }
//        double getX1() const { return X1; }
//        void setX1(double value) { X1 = value; }

//        QString getX2String() const { return QString::number(X2); }
//        double getX2() const { return X2; }
//        void setX2(double value) { X2 = value; }

//        QString getY1String() const { return QString::number(Y1); }
//        double getY1() const { return Y1; }
//        void setY1(double value) { Y1 = value; }

//        QString getY2String() const { return QString::number(Y2); }
//        double getY2() const { return Y2; }
//        void setY2(double value) { Y2 = value; }

//        double getZ1() const { return Z1; }
//        void setZ1(double value) { Z1 = value; }

//        QString getNumRowsString() const { return QString::number(numRows); }
//        int getNumRows() const { return numRows; }
//        void incrementNumRow() { numRows += 1; }

//        QString getCurrentRowString() const { return QString::number(currentRow); }
//        int getCurrentRow() const { return currentRow; }
//        void setCurrentRow(int value) { currentRow = value; }

//        QString getCurrentCycleString() const { return QString::number(currentCycle); }
//        int getCurrentCycle() const { return currentCycle; }
//        void setCurrentCycle(int value) { currentCycle = value; }
//        void incrementCurrentCycle() { currentCycle += 1; }

//        QString getScanSpeedString() const { return QString::number(scanSpeed); }
//        void setRecipeScanSpeed() {
//            bool ok;
//            scanSpeed = recipe.getSpeed().toDouble(&ok);
//        }

//        QString getScanCyclesString() const { return QString::number(scanCycles); }
//        int getScanCycles() const { return scanCycles; }
//        void setRecipeScanCycles() {
//            bool ok;
//            scanCycles = recipe.getCycles().toInt(&ok);
//        }

//        QString getminXString() const { return QString::number(minX); }
//        QString getmaxXString() const { return QString::number(maxX); }
//        QString getminYString() const { return QString::number(minY); }
//        QString getmaxYString() const { return QString::number(maxY); }
//        void determineSubstrateArea() {
//            double x1 = stageBaseCoordinates.translateCoordX_PH_2Base(X1);
//            double x2 = stageBaseCoordinates.translateCoordX_PH_2Base(X2);
//            // Determine the minimum and maximum x values
//            minX = std::min(x1, x2);
//            maxX = std::max(x1, x2);

//            double y1 = stageBaseCoordinates.translateCoordY_PH_2Base(Y1);
//            double y2 = stageBaseCoordinates.translateCoordY_PH_2Base(Y2);
//            // Determine the minimum and maximum y values
//            minY = std::min(y1, y2);
//            maxY = std::max(y1, y2);

//        }

//        QString getminZString() const { return QString::number(minZ); }
//        QString getmaxZString() const { return QString::number(maxZ); }
//        void determineStageZMinMax() {
//            bool ok;
//            // Determine the minimum and maximum z values
//            minZ = stage.getPinsBuriedPos();
//            maxZ = stageBaseCoordinates.getZ() - recipe.getThickness().toDouble(&ok) - recipe.getGap().toDouble(&ok);
//        }
//        QString getScanSweepString () const {
//            return QString::number(scanSweep);
//        }
//        void determineScanSweep() {
//            bool ok;
//            scanSweep = plasmahead.getPlasmaHeadSlitLength() - recipe.getOverlap().toDouble(&ok);
//        }

//        void calculateTotalScanXY() {
//            totalXScan = maxX - minX;
//            totalYScan = maxY - minY;
//        }

//        void calculateNumScanSweeps() {
//            remainingX = totalXScan;
//            while (remainingX > 0) {
//                incrementNumRow();
//                remainingX -= scanSweep;
//            }
//        }
//        QString getStartingXPositionString () const {
//            return QString::number(startingXPosition);
//        }
//        QString getStartingYPositionString () const {
//            return QString::number(startingYPosition);
//        }
//        QString getEndingYPositionString () const {
//            return QString::number(endingYPosition);
//        }
//        double getStartingXPosition() const { return startingXPosition; }
//        void calculateStartingPositions() {
//            if (totalXScan <= scanSweep) {
//                startingXPosition = (maxX + minX) / 2;
//            }
//            else {
//                startingXPosition = maxX - (scanSweep / 2);
//            }
//            startingYPosition = maxY + plasmahead.getPlasmaHeadSlitLength();
//            endingYPosition = minY + plasmahead.getPlasmaHeadSlitLength();
//        }
//        double getNextSweepStartingPosition() const { return nextSweepStartingPosition; }
//        void setNextSweepStartingPosition() {
//            if (getCurrentRow() > 1) {
//                nextSweepStartingPosition = getStartingXPosition() - (scanSweep);
//            }
//        }

//        bool finishedScanning() {
//            if (getCurrentRow() > getNumRows()) {
//                return true;
//            }
//            else {
//                return false;
//            }
//        }
//        bool shouldContinueScanning() {
//            return getCurrentCycle() >= getScanCycles();
//        }

////    } ScanSM;



