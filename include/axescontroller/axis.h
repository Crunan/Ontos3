#ifndef AXIS_H
#define AXIS_H

#include "coordinate.h"

#include <QObject>

//Axes SM
enum AxisState {
    AXIS_IDLE = 0x10,
    AXIS_START = 0x11,
    AXIS_WAIT = 0x12,
    AXIS_REL_START = 0x13,
    AXIS_REL_WAIT = 0x14,
    AXIS_JOY_ON = 0x15
};


class Axis : public QObject {
    Q_OBJECT

public:
    Axis(QObject* parent = nullptr);
    ~Axis();

    int getCurrentState() const { return m_currentState; }
    void setCurrentState(const QString state);

    void setCurrentError(const QString error);
    int getError() const { return m_currentError; }

    void setCurrentPosition(const QString position);
    double getPosition() const { return m_currentPosition; }

    void setMaxPos(const double maxPos) { m_maxPos = maxPos; }
    void setMaxSpeed(const double maxSpeed) { m_maxSpeed = maxSpeed; }
    void setHomePos(const double homePos) { m_homePos = homePos; }

    double getMaxPos() const { return m_maxPos; }
    double getMaxSpeed() const { return m_maxSpeed; }
    double getHomePos() const { return m_homePos; }

    QString getMaxPosQStr() const { return QString::number(m_maxPos, 'f', 2); }
    QString getMaxSpeedQStr() const { return QString::number(m_maxSpeed, 'f', 2); }
    QString getHomePosQStr() const { return QString::number(m_homePos, 'f', 2); }


    void setp2Base(QString p2Base) { m_P_2_Base = p2Base.toDouble(); }
    double getp2Base() const { return m_P_2_Base; }

    void setScanMax(double max) { m_scanMax = max; }
    void setScanMin(double min) { m_scanMin = min; }

    double getScanMax() const { return m_scanMax; }
    double getScanMin() const { return m_scanMin; }

    double setTwoSpotFirstPoint(double first) { m_twoSpotFirstPoint = first; }
    double setTwoSpotSecondPoint(double second) { m_twoSpotSecondPoint = second; }

    void checkAndSetDimensions();

    //void setCurrentState(AxisState state);

signals:
    void currentStateChanged();
    void errorChanged();
    void postionChanged(QString pos);

private:
    // AxisState currentState;
    int m_currentState;
    int m_currentError;
    //Coordinate position;
    double m_currentPosition;

    double m_maxPos;
    double m_maxSpeed;
    double m_homePos;
    double m_P_2_Base;

    double m_twoSpotFirstPoint;
    double m_twoSpotSecondPoint;

    double m_scanMax;
    double m_scanMin;

};

#endif // AXIS_H