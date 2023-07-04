#ifndef PLASMA_RECIPE_H
#define PLASMA_RECIPE_H

#include <QObject>

class PlasmaRecipe : public QObject {
    Q_OBJECT    
    Q_PROPERTY(double gas1Setpoint READ getGas1Setpoint WRITE setGas1Setpoint NOTIFY gas1SetpointChanged)
    Q_PROPERTY(double gas2Setpoint READ getGas2Setpoint WRITE setGas2Setpoint NOTIFY gas2SetpointChanged)
    Q_PROPERTY(double gas3Setpoint READ getGas3Setpoint WRITE setGas3Setpoint NOTIFY gas3SetpointChanged)
    Q_PROPERTY(double gas4Setpoint READ getGas4Setpoint WRITE setGas4Setpoint NOTIFY gas4SetpointChanged)
    Q_PROPERTY(int RfSetpoint READ getRfSetpoint WRITE setRfSetpoint NOTIFY RfSetpointChanged)
    Q_PROPERTY(double tunerSetpoint READ getTunerSetpoint WRITE setTunerSetpoint NOTIFY tunerSetpointChanged)
    Q_PROPERTY(bool autoTuneOn READ isAutoTuneOn WRITE setAutoTuneOn NOTIFY autoTuneOnChanged)

private:    
    double gas1Setpoint_;
    double gas2Setpoint_;
    double gas3Setpoint_;
    double gas4Setpoint_;
    int RfSetpoint_;
    double tunerSetpoint_;
    bool autoTuneOn_;

public:
    PlasmaRecipe(QObject* parent = nullptr);
    ~PlasmaRecipe();

    double getGas1Setpoint() const;
    void setGas1Setpoint(double setpoint);

    double getGas2Setpoint() const;
    void setGas2Setpoint(double setpoint);

    double getGas3Setpoint() const;
    void setGas3Setpoint(double setpoint);

    double getGas4Setpoint() const;
    void setGas4Setpoint(double setpoint);

    int getRfSetpoint() const;
    void setRfSetpoint(int setpoint);

    double getTunerSetpoint() const;
    void setTunerSetpoint(double setpoint);

    bool isAutoTuneOn() const;
    void setAutoTuneOn(bool enabled);

signals:
    void gas1SetpointChanged();
    void gas2SetpointChanged();
    void gas3SetpointChanged();
    void gas4SetpointChanged();
    void RfSetpointChanged();
    void tunerSetpointChanged();
    void autoTuneOnChanged();
};

#endif // PLASMA_RECIPE_H
