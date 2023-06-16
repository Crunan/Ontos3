#ifndef TUNER_H
#define TUNER_H

#include <QObject>

struct Tuner : public QObject {

    Q_OBJECT
    Q_PROPERTY(double currentPosition READ getPosition CONSTANT WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(double loadedSetpoint READ getSetpoint CONSTANT WRITE setSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(double autoTune READ getAutoTune CONSTANT WRITE toggleAutoTune NOTIFY autoTuneChanged)

public:
    Tuner();

    double currentPosition;
    double loadedSetpoint;
    bool autoTune;

public:
    double getPosition() const;
    void setPosition(double position);

    double getSetpoint() const;
    void setSetpoint(double setpoint);

    double getAutoTune() const;
    void toggleAutoTune();

signals:
    void positionChanged();
    void setpointChanged();
    void autoTuneChanged();


};
#endif // TUNER_H
