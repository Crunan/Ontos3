//#include "include/plasmacontroller/tuner.h"

//Tuner::Tuner()
//    : currentPosition(0.0), loadedSetpoint(0.0), autoTune(true)
//{
//}

//double Tuner::getPosition() const
//{
//    return currentPosition;
//}

//void Tuner::setPosition(double newPosition)
//{
//    if (currentPosition != newPosition)
//    {
//        currentPosition = newPosition;
//        emit positionChanged();
//    }
//}

//void Tuner::toggleAutoTune(double value) {
//    // Implementation of the toggleAutoTune function
//    // using the provided 'value' parameter
//    // ...
//}

////struct TUNER_POS {
//    //    public:
//    //    double m_loadedSP;
//    //    double m_actualPos;
//    //    double m_actualPosPct;
//    //    bool m_autoTune;
//    //    bool m_autoTuneCMDFlag;
//    //    bool m_readyToLoad;

//    //    public:
//    //    void setReadyToLoad(bool toggle) {
//    //        m_readyToLoad = toggle;
//    //    }
//    //    bool getReadyToLoad() {
//    //        return m_readyToLoad;
//    //    }
//    //    void setAutoTuneCMDFlag(bool toggle) {
//    //        m_autoTuneCMDFlag = toggle;
//    //    }
//    //    void setLoadedValue(QString value) {
//    //        bool ok;
//    //        m_loadedSP = value.toDouble(&ok);
//    //        this->setReadyToLoad(true);
//    //    }
//    //    void setAutoMode(QString value) {
//    //        bool ok;
//    //        m_autoTune = value.toInt(&ok);
//    //        this->setAutoTuneCMDFlag(true);
//    //    }
//    //    void setActualPosition(QString pos) {
//    //        bool ok;
//    //        m_actualPos = pos.toDouble(&ok);
//    //    }

//    //    double getLoadedSP() {
//    //        return m_loadedSP;
//    //    }
//    //    int getLoadedSPInteger() {
//    //        return int(m_loadedSP);
//    //    }
//    //    bool getAutoTune() {
//    //        return m_autoTune;
//    //    }
//    //    double getActualPosition() {
//    //        return m_actualPos;
//    //    }
//    //    QString getLoadedSPQStr() {
//    //        return QString::number(m_loadedSP);
//    //    }
//    //    QString getAutoTuneQStr() {
//    //        return QString::number(m_autoTune);
//    //    }
//    //    bool getAutoTuneCMDFlag() {
//    //        return m_autoTuneCMDFlag;
//    //    }
//    //} TUNER;
