#ifndef AXIS_H
#define AXIS_H

#include <QObject>

//Axis states
enum AxisState {
    AXIS_IDLE = 0x10,
    AXIS_START = 0x11,
    AXIS_WAIT = 0x12,
    AXIS_REL_START = 0x13,
    AXIS_REL_WAIT = 0x14,
    AXIS_JOY_ON = 0x15
};

// constants used for axis commands
const int XAXIS_COMMAND_NUM = 0;
const int YAXIS_COMMAND_NUM = 1;
const int ZAXIS_COMMAND_NUM = 2;

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
    QString getPositionQStr() const { return QString::number(m_currentPosition, 'f', 2); }

    void saveMaxPos(const double maxPos) { m_maxPos = maxPos; }
    void saveMaxSpeed(const double maxSpeed) { m_maxSpeed = maxSpeed; }
    void saveHomePos(const double homePos) { m_homePos = homePos; }

    double getMaxPos() const { return m_maxPos; }
    double getMaxSpeed() const { return m_maxSpeed; }
    double getHomePos() const { return m_homePos; }

    QString getMaxPosQStr() const { return QString::number(m_maxPos, 'f', 2); }
    QString getMaxSpeedQStr() const { return QString::number(m_maxSpeed, 'f', 2); }
    QString getHomePosQStr() const { return QString::number(m_homePos, 'f', 2); }

    // base coords
    void setScanMaxBase(double maxBase) { m_scanMaxBase = maxBase; }
    void setScanMinBase(double minBase) { m_scanMinBase = minBase; }
    double getScanMaxBase() const { return m_scanMaxBase; }
    double getScanMinBase() const { return m_scanMinBase; }

    void checkAndSetDimensions();

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
    // Base coordinates
    double m_scanMinBase;
    double m_scanMaxBase;
};

#endif // AXIS_H
