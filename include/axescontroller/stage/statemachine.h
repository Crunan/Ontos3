#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QObject>


class StateMachine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StageState state READ getState WRITE setState NOTIFY stateChanged)

public:
    enum StageState {
        IDLE,
        MOVING,
        STOPPED,
        ERROR
    };
    Q_ENUM(StageState)

private:
    StageState currentState;

public:
    StateMachine(QObject* parent = nullptr);
    ~StateMachine();

    StageState getState() const;
    void setState(StageState state);   
signals:
    void stateChanged();
};


#endif // STATEMACHINE_H
