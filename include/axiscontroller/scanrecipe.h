#ifndef SCAN_RECIPE_H
#define SCAN_RECIPE_H

#include <QObject>

class ScanRecipe : public QObject {
    Q_OBJECT
    Q_PROPERTY(double substrateThickness READ getSubstrateThickness WRITE setSubstrateThickness NOTIFY substrateThicknessChanged)
    Q_PROPERTY(double plasmaHeadGap READ getPlasmaHeadGap WRITE setPlasmaHeadGap NOTIFY plasmaHeadGapChanged)
    Q_PROPERTY(double substrateOverlap READ getSubstrateOverlap WRITE setSubstrateOverlap NOTIFY substrateOverlapChanged)
    Q_PROPERTY(double stageSpeed READ getStageSpeed WRITE setStageSpeed NOTIFY stageSpeedChanged)
    Q_PROPERTY(int numCycles READ getNumCycles WRITE setNumCycles NOTIFY numCyclesChanged)
    Q_PROPERTY(bool autoScanOn READ isAutoScanOn WRITE setAutoScanOn NOTIFY autoScanOnChanged)
    Q_PROPERTY(bool N2PurgeOn READ isN2PurgeOn WRITE setN2PurgeOn NOTIFY N2PurgeOnChanged)

private:
    double substrateThickness_;
    double plasmaHeadGap_;
    double substrateOverlap_;
    double stageSpeed_;
    int numCycles_;
    bool autoScanOn_;
    bool N2PurgeOn_;

public:
    ScanRecipe(QObject* parent = nullptr);
    ~ScanRecipe();

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

#endif // SCAN_RECIPE_H
