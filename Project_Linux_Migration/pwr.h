#ifndef PWR_H
#define PWR_H

#include <QObject>

struct PWR : public QObject {

    Q_OBJECT
    Q_PROPERTY(double loadedSetpoint READ getLoadedSetpoint CONSTANT WRITE setLoadedSetpoint NOTIFY loadedSetpointChanged)
    Q_PROPERTY(double forwardWatts READ getForwardWatts CONSTANT NOTIFY forwardWattsChanged)
    Q_PROPERTY(double reflectedWatts READ getReflectedWatts CONSTANT NOTIFY reflectedWattsChanged)


public:
    PWR();

    int loadedSetpoint;
    double forwardWatts;
    double reflectedWatts;
    int maxWatts;

public:
    double getLoadedSetpoint() const;
    void setLoadedSetpoint(int value);

    double getForwardWatts() const;
    double getReflectedWatts() const;

signals:
    void loadedSetpointChanged();
    void forwardWattsChanged();
    void reflectedWattsChanged();

};

//    public:
//    int m_loadedSetPoint;
//    int m_actualPForward;
//    int m_actualPReflected;
//    int m_maxWatts;
//    bool m_readyToLoad;


//    public:
//    void setReadyToLoad(bool toggle) {
//        m_readyToLoad = toggle;
//    }
//    bool getReadyToLoad() {
//        return m_readyToLoad;
//    }
//    void setLoadedSetPoint(QString value) {
//        bool ok;
//        m_loadedSetPoint = value.toInt(&ok);
//        this->setReadyToLoad(true);
//    }
//    void setMaxWatts(QString max) {
//        bool ok;
//        m_maxWatts = max.toInt(&ok);
//    }
//    void setActualWatts(QString watts) {
//        bool ok;
//        m_actualPForward = watts.toInt(&ok, 10);
//    }
//    void setActualRefWatts(QString watts) {
//        bool ok;
//        m_actualPReflected = watts.toInt(&ok, 10);
//    }
//    int getLoadedSetPoint() {
//        return m_loadedSetPoint;
//    }
//    QString getLoadedSetPointQStr() {
//        return QString::number(m_loadedSetPoint);
//    }
//    int getActualWatts() {
//        return m_actualPForward;
//    }
//    int getReflectedWatts() {
//        return m_actualPReflected;
//    }
//} RF;
#endif // PWR_H
