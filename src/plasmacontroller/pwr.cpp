//#include "include/plasmacontroller/pwr.h"

//PWR::PWR()
//    : loadedSetpoint(0), forwardWatts(0.0), reflectedWatts(0.0), maxWatts(0)
//{
//}

//double PWR::getLoadedSetpoint() const
//{
//    return loadedSetpoint;
//}

//void PWR::setLoadedSetpoint(int value)
//{
//    if (loadedSetpoint != value)
//    {
//        loadedSetpoint = value;
//        emit loadedSetpointChanged();
//    }
//}
//double PWR::getForwardWatts() const
//{
//    return forwardWatts;
//}

//double PWR::getReflectedWatts() const
//{
//    return reflectedWatts;
//}


////struct PWR {
////    public:
////    int m_loadedSetPoint;
////    int m_actualPForward;
////    int m_actualPReflected;
////    int m_maxWatts;
////    bool m_readyToLoad;


////    public:
////    void setReadyToLoad(bool toggle) {
////        m_readyToLoad = toggle;
////    }
////    bool getReadyToLoad() {
////        return m_readyToLoad;
////    }
////    void setLoadedSetPoint(QString value) {
////        bool ok;
////        m_loadedSetPoint = value.toInt(&ok);
////        this->setReadyToLoad(true);
////    }
////    void setMaxWatts(QString max) {
////        bool ok;
////        m_maxWatts = max.toInt(&ok);
////    }
////    void setActualWatts(QString watts) {
////        bool ok;
////        m_actualPForward = watts.toInt(&ok, 10);
////    }
////    void setActualRefWatts(QString watts) {
////        bool ok;
////        m_actualPReflected = watts.toInt(&ok, 10);
////    }
////    int getLoadedSetPoint() {
////        return m_loadedSetPoint;
////    }
////    QString getLoadedSetPointQStr() {
////        return QString::number(m_loadedSetPoint);
////    }
////    int getActualWatts() {
////        return m_actualPForward;
////    }
////    int getReflectedWatts() {
////        return m_actualPReflected;
////    }
////} RF;
