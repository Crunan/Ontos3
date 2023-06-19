#ifndef LIGHTTOWERSTATEMACHINE_H
#define LIGHTTOWERSTATEMACHINE_H

#include <QObject>

struct LightTowerStateMachine : public QObject {

    Q_OBJECT
    Q_PROPERTY(LightState currentState READ getState WRITE setState NOTIFY stateChanged)


public:
    //Light SM
    enum LightState {
        INACTIVE,
        ERROR,
        READY,
        ACTIVE
    };


    LightState currentState;

public:
    LightTowerStateMachine();

    LightState getState() const;
    void setState(LightState state);

signals:
    void stateChanged();

};

#endif // LIGHTTOWERSTATEMACHINE_H
