#ifndef STAGESTATEMACHINE_H
#define STAGESTATEMACHINE_H

#include <QObject>

class StageStateMachine : public QObject
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
    QList<double> movementSequence;

public:
    StageStateMachine(QObject* parent = nullptr);
    ~StageStateMachine();

    StageState getState() const;
    void setState(StageState state);

    // Methods to control the stage movements
    void move(double position);
    void moveSequence(const QList<double>& sequence);
    void stop();
    void emergencyStop();
signals:
    void stateChanged();
};

#endif // STAGESTATEMACHINE_H
