#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>
#include "filereader.h"
#include "plasmarecipe.h"
//#include "include/axiscontroller/scanrecipe.h"

class Recipe : public QObject {
    Q_OBJECT

public:
    Recipe(QObject* parent = nullptr);
    ~Recipe();

    void setAutoScanFlag(bool toggle) { m_autoScanFlag = toggle; }
    bool getAutoScanFlag() const { return m_autoScanFlag; }

    void setAutoScan(bool toggle) { m_autoScan = toggle; }
    QString getAutoScan() const { return QString::number(m_autoScan); }
    bool getAutoScanBool() const { return m_autoScan; }

    void setPurge(bool toggle) { m_N2PurgeRecipe = toggle; }
    bool getPurge() const { return m_N2PurgeRecipe; }
    QString getPurgeQStr() const { return QString::number(m_N2PurgeRecipe); }

    void setCycles(int cycles) { m_cycles = cycles; }
    QString getCycles() { return QString::number(m_cycles); }
    int getCyclesInt() { return m_cycles; }

    void setSpeed(double speed) { m_speed = speed; }
    QString getSpeed() { return QString::number(m_speed); }

    void setOverlap(double overlap) { m_overlap = overlap; }
    QString getOverlap() { return QString::number(m_overlap); }

    void setGap(double gap) { m_gap = gap; }
    QString getGap() { return QString::number(m_gap); }

    void setThickness(double thickness) { m_thickness = thickness; }
    QString getThickness() { return QString::number(m_thickness); }

    //PlasmaRecipe getPlasmaRecipe() const { return m_plasmaRecipe; }
    //ScanRecipe getScanRecipe() const { return m_scanRecipe; }

private:
    //PlasmaRecipe m_plasmaRecipe;
    //ScanRecipe m_scanRecipe;

    bool m_autoScanFlag;
    bool m_autoScan;
    bool m_N2PurgeRecipe;
    int m_cycles;
    double m_speed;
    double m_overlap;
    double m_gap;
    double m_thickness;

};

#endif // RECIPE_H
