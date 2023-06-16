#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>

class Recipe : public QObject {
    Q_OBJECT
    //Recipe Part 1 - Plasma
    Q_PROPERTY(double gas1Setpoint READ gas1Setpoint WRITE setGas1Setpoint NOTIFY gas1SetpointChanged)
    Q_PROPERTY(double gas2Setpoint READ gas2Setpoint WRITE setGas2Setpoint NOTIFY gas2SetpointChanged)
    Q_PROPERTY(double gas3Setpoint READ gas3Setpoint WRITE setGas3Setpoint NOTIFY gas3SetpointChanged)
    Q_PROPERTY(double gas4Setpoint READ gas4Setpoint WRITE setGas4Setpoint NOTIFY gas4SetpointChanged)
    Q_PROPERTY(int RfSetpoint READ RfSetpoint WRITE setRfSetpoint NOTIFY RfSetpointChanged)
    Q_PROPERTY(double tunerSetpoint READ tunerSetpoint WRITE setTunerSetpoint NOTIFY tunerSetpointChanged)
    Q_PROPERTY(bool autoTuneOn READ isAutoTuneOn WRITE setAutoTuneOn NOTIFY autoTuneOnChanged)

    //Recipe Part 2 - Stage & Substrate
    Q_PROPERTY(double substrateThickness READ getSubstrateThickness WRITE setSubstrateThickness NOTIFY substrateThicknessChanged)
    Q_PROPERTY(double plasmaHeadGap READ getPlasmaHeadGap WRITE setPlasmaHeadGap NOTIFY plasmaHeadGapChanged)
    Q_PROPERTY(double substrateOverlap READ getSubstrateOverlap WRITE setSubstrateOverlap NOTIFY substrateOverlapChanged)
    Q_PROPERTY(double stageSpeed READ getStageSpeed WRITE setStageSpeed NOTIFY stageSpeedChanged)
    Q_PROPERTY(int numCycles READ getNumCycles WRITE setNumCycles NOTIFY numCyclesChanged)
    Q_PROPERTY(bool autoScanOn READ isAutoScanOn WRITE setAutoScanOn NOTIFY autoScanOnChanged)
    Q_PROPERTY(bool N2PurgeOn READ isN2PurgeOn WRITE setN2PurgeOn NOTIFY N2PurgeOnChanged)
private:
    //Recipe Part 1 - Plasma
    double gas1Setpoint;
    double gas2Setpoint;
    double gas3Setpoint;
    double gas4Setpoint;
    int RfSetpoint;
    double tunerSetpoint;
    bool autoTuneOn;
    //Recipe Part 2 - Stage & Substrate
    double substrateThickness;
    double plasmaHeadGap;
    double substrateOverlap;
    double stageSpeed;
    int numCycles;
    bool autoScanOn;
    bool N2PurgeOn;

public:
    Recipe(QObject* parent = nullptr);
    ~Recipe();

    //Recipe Part 1 - Plasma
    double gas1Setpoint() const;
    void setGas1Setpoint(double setpoint);

    double gas2Setpoint() const;
    void setGas2Setpoint(double setpoint);

    double gas3Setpoint() const;
    void setGas3Setpoint(double setpoint);

    double gas4Setpoint() const;
    void setGas4Setpoint(double setpoint);

    int RfSetpoint() const;
    void setRfSetpoint(int setpoint);

    double tunerSetpoint() const;
    void setTunerSetpoint(double setpoint);

    bool isAutoTuneOn() const;
    void setAutoTuneOn(bool enabled);

    //Recipe Part 2 - Stage & Substrate
    double getSubstrateThickness() const;
    void setSubstrateThickness(double thickness);

    double getPlasmaHeadGap() const;
    void setPlasmaHeadGap(double gap);

    double getSubstrateOverlap() const;
    void setSubstrateOverlap(double overlap);

    double getStageSpeed() const;
    void setStageSpeed(double speed);

    int getNumCycles() const;
    void setNumCycles(int cycles);

    bool isAutoScanOn() const;
    void setAutoScanOn(bool enabled);

    bool isN2PurgeOn() const;
    void setN2PurgeOn(bool enabled);
signals:
    //Recipe Part 1 - Plasma
    void gas1SetpointChanged();
    void gas2SetpointChanged();
    void gas3SetpointChanged();
    void gas4SetpointChanged();
    void RfSetpointChanged();
    void tunerSetpointChanged();
    void autoTuneOnChanged();
    //Recipe Part 2 - Stage & Substrate
    void substrateThicknessChanged();
    void plasmaHeadGapChanged();
    void substrateOverlapChanged();
    void stageSpeedChanged();
    void numCyclesChanged();
    void autoScanOnChanged();
    void N2PurgeOnChanged();
};

#endif // RECIPE_H
