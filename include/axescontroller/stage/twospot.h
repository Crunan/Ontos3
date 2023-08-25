#ifndef TWOSPOT_H
#define TWOSPOT_H


class TwoSpot
{
public:
    TwoSpot();
};

#endif //TWOSPOT_H
////Two Spot SM
//enum TwoSpotAxesStates {
//    TSSM_SHUTDOWN,
//    TSSM_IDLE,
//    TSSM_STARTUP,
//    TSSM_GET_FIRST,
//    TSSM_WAIT_JOY_BTN_OFF,
//    TSSM_GET_SECOND
//};
// TWOSPOT_H
//struct TWOSPOTAXESSTATEMACHINE {
//    TwoSpotAxesStates m_state;
//    TwoSpotAxesStates external_new_state;
//    bool external_state_change;

//    double m_firstXpos;
//    double m_firstYpos;
//    double m_secondXpos;
//    double m_secondYpos;

//    TwoSpotAxesStates getState() {
//        return this->m_state;
//    }
//    void setState(TwoSpotAxesStates state) {
//        this->m_state = state;
//    }

//    public:
//    double getFirstX() {
//        return m_firstXpos;
//    }
//    double getFirstY() {
//        return m_firstYpos;
//    }
//    double getSecondX() {
//        return m_secondXpos;
//    }
//    double getSecondY() {
//        return m_secondYpos;
//    }
//    void setFirstX(double x) {
//        m_firstXpos = x;
//    }
//    void setFirstY(double y) {
//        m_firstYpos = y;
//    }
//    void setSecondX(double x) {
//        m_secondXpos = x;
//    }
//    void setSecondY(double y) {
//        m_secondYpos = y;
//    }
//    void checkXDimensions() {
//        if (m_firstXpos > m_secondXpos) {
//            ScanSM.setX2(m_firstXpos);
//            ScanSM.setX1(m_secondXpos);
//        }
//        else {
//            ScanSM.setX1(m_firstXpos);
//            ScanSM.setX2(m_secondXpos);
//        }
//    }
//    void checkYDimensions() {
//        if (m_firstYpos > m_secondYpos) {
//            ScanSM.setY2(m_firstYpos);
//            ScanSM.setY1(m_secondYpos);
//        }
//        else {
//            ScanSM.setY1(m_firstYpos);
//            ScanSM.setY2(m_secondYpos);
//        }
//    }
//} TwoSpotSM;
