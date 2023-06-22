#ifndef TUNER_H
#define TUNER_H

#include <QObject>

class Tuner : public QObject {

    Q_OBJECT
    Q_PROPERTY(double currentPosition READ getCurrentPosition WRITE setCurrentPosition NOTIFY positionChanged)
    Q_PROPERTY(double loadedSetpoint READ getLoadedSetpoint WRITE setLoadedSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(bool autoTune READ getAutoTune WRITE setAutoTune NOTIFY autoTuneChanged)

public:
    Tuner();

    double getCurrentPosition() const;
    void setCurrentPosition(double position);

    double getLoadedSetpoint() const;
    void setLoadedSetpoint(double setpoint);

    bool getAutoTune() const;
    void setAutoTune(bool state);
    void toggleAutoTune();

signals:
    void positionChanged();
    void setpointChanged();
    void autoTuneChanged();

private:
    double currentPosition_;
    double loadedSetpoint_;
    bool autoTune_;
};

#endif // TUNER_H
