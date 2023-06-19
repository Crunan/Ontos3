#ifndef TUNER_H
#define TUNER_H

#include <QObject>

class Tuner : public QObject {

    Q_OBJECT
    Q_PROPERTY(double currentPosition READ getPosition WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(double loadedSetpoint READ getSetpoint WRITE setSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(bool autoTune READ getAutoTune WRITE setAutoTune NOTIFY autoTuneChanged)

public:
    Tuner();

    double currentPosition;
    double loadedSetpoint;
    bool autoTune;


    double getPosition() const;
    void setPosition(double position);

    double getSetpoint() const;
    void setSetpoint(double setpoint);

    bool getAutoTune() const;
    void setAutoTune(bool state);
    void toggleAutoTune();

signals:
    void positionChanged();
    void setpointChanged();
    void autoTuneChanged();
public slots:

};
#endif // TUNER_H
