#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>

class Recipe : public QObject {
    Q_OBJECT
    //Recipe Part 1 - Plasma
    Q_PROPERTY(double gas1Setpoint READ getGas1Setpoint WRITE setGas1Setpoint NOTIFY gas1SetpointChanged)
    Q_PROPERTY(double gas2Setpoint READ getGas2Setpoint WRITE setGas2Setpoint NOTIFY gas2SetpointChanged)
    Q_PROPERTY(double gas3Setpoint READ getGas3Setpoint WRITE setGas3Setpoint NOTIFY gas3SetpointChanged)
    Q_PROPERTY(double gas4Setpoint READ getGas4Setpoint WRITE setGas4Setpoint NOTIFY gas4SetpointChanged)
    Q_PROPERTY(int RfSetpoint READ getRfSetpoint WRITE setRfSetpoint NOTIFY RfSetpointChanged)
    Q_PROPERTY(double tunerSetpoint READ getTunerSetpoint WRITE setTunerSetpoint NOTIFY tunerSetpointChanged)
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
    double m_gas1Setpoint;
    double m_gas2Setpoint;
    double m_gas3Setpoint;
    double m_gas4Setpoint;
    int m_RfSetpoint;
    double m_tunerSetpoint;
    bool m_autoTuneOn;
    //Recipe Part 2 - Stage & Substrate
    double m_substrateThickness;
    double m_plasmaHeadGap;
    double m_substrateOverlap;
    double m_stageSpeed;
    int m_numCycles;
    bool m_autoScanOn;
    bool m_N2PurgeOn;

public:
    Recipe(QObject* parent = nullptr);
    ~Recipe();

    //Recipe Part 1 - Plasma
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
