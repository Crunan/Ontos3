#ifndef AXIS_H
#define AXIS_H

#include "coordinate.h"

#include <QObject>

//Axes SM
enum AxisState {
    IDLE = 0x10,
    START = 0x11,
    WAIT = 0x12,
    REL_START = 0x13,
    REL_WAIT = 0x14,
    JOY_ON = 0x15
};

class Axis : public QObject {
    Q_OBJECT
    Q_PROPERTY(AxisState currentState READ getCurrentState CONSTANT WRITE setCurrentState NOTIFY currentStateChanged)
    Q_PROPERTY(int error READ getError CONSTANT NOTIFY errorChanged)

private:
    AxisState currentState;
    int error;
    Coordinate position;

public:
    Axis(QObject* parent = nullptr);
    ~Axis();

    AxisState getCurrentState() const;
    void setCurrentState(AxisState state);

    int getError() const;
    double getPosition() const;

signals:
    void currentStateChanged();
    void errorChanged();

};

#endif // AXIS_H
