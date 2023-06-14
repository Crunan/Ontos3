#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>

class Recipe : public QObject {
    Q_OBJECT
    Q_PROPERTY(double substrateThickness READ getSubstrateThickness WRITE setSubstrateThickness NOTIFY substrateThicknessChanged)
    Q_PROPERTY(double plasmaHeadGap READ getPlasmaHeadGap WRITE setPlasmaHeadGap NOTIFY plasmaHeadGapChanged)
    Q_PROPERTY(double substrateOverlap READ getSubstrateOverlap WRITE setSubstrateOverlap NOTIFY substrateOverlapChanged)
    Q_PROPERTY(double stageSpeed READ getStageSpeed WRITE setStageSpeed NOTIFY stageSpeedChanged)
    Q_PROPERTY(int numCycles READ getNumCycles WRITE setNumCycles NOTIFY numCyclesChanged)
    Q_PROPERTY(bool autoScanOn READ isAutoScanOn WRITE setAutoScanOn NOTIFY autoScanOnChanged)
    Q_PROPERTY(bool N2PurgeOn READ isN2PurgeOn WRITE setN2PurgeOn NOTIFY N2PurgeOnChanged)

private:
    double substrateThickness;
    double plasmaHeadGap;
    double substateOverlap;
    double stageSpeed;
    int numCycles;
    bool autoScanOn;
    bool N2PurgeOn;

public:
    Recipe(QObject* parent = nullptr);
    ~Recipe();

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
    void substrateThicknessChanged();
    void plasmaHeadGapChanged();
    void substrateOverlapChanged();
    void stageSpeedChanged();
    void numCyclesChanged();
    void autoScanOnChanged();
    void N2PurgeOnChanged();
};

#endif // RECIPE_H
