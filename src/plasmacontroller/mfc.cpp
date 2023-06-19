//#include "include/plasmacontroller/mfc.h"

//MFC::MFC()
//    : loadedSetpoint(0.0), actualFlow(0.0), range(0.0)
//{

//}

//double MFC::getSetpoint() const
//{
//    return loadedSetpoint;
//}

//void MFC::setSetpoint(double value)
//{
//    if (loadedSetpoint != value)
//    {
//        loadedSetpoint = value;
//        emit setpointChanged();
//    }
//}

//double MFC::getRange() const
//{
//    return range;
//}

//double MFC::getFlow() const
//{
//    return actualFlow;
//}

////struct MFController {
////public:
////    double m_actualFlow;
////    double m_loadedFlow;
////    double m_range;
////    bool m_readyToLoad;

////    public:

////    void setReadyToLoad(bool toggle) {
////        m_readyToLoad = toggle;
////    }
////    bool getReadyToLoad() {
////        return m_readyToLoad;
////    }
////    void setLoadedFlow(QString flow) {
////        bool ok;
////        m_loadedFlow = flow.toDouble(&ok);
////        this->setReadyToLoad(true);
////    }
////    void setRange(QString range) {
////        bool ok;
////        m_range = range.toDouble(&ok);
////    }
////    void setActualFlow(QString flow) {
////        bool ok;
////        m_actualFlow = flow.toDouble(&ok);
////    }
////    QString getLoadedFlowQStr() {
////        return QString::number(m_loadedFlow, 'f', 2);
////    }
////    QString getRange() {
////        return QString::number(m_range, 'f', 2);
////    }
////    QString getActualFlow() {
////        return QString::number(m_actualFlow, 'f', 2);
////    }
////    int getActualFlowInt() {
////        return int(m_actualFlow);
////    }
////    double getLoadedFlow() {
////        return m_loadedFlow;
////    }

////} MFC[5];
