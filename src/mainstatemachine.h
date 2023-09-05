#ifndef MAINSTATEMACHINE_H
#define MAINSTATEMACHINE_H

#include <QObject>
//#include <QtStateMachine/QState>
//#include <QtStateMachine/QFinalState>

struct MainStateMachine { //: public QStateMachine

    Q_OBJECT
    Q_PROPERTY(SystemState currentState READ getState WRITE setState NOTIFY stateChanged)


public:
    //Main State machine
    enum SystemState {
        STARTUP,
        POLLING,
        IDLE,
        SHUTDOWN
    };

    SystemState currentState;

public:
    SystemState getState() const;
    void setState(SystemState state);

signals:
    void stateChanged();

};

#endif // MAINSTATEMACHINE_H
