#ifndef PLASMARECIPE_H
#define PLASMARECIPE_H

#include "filereader.h"
#include "plasmacontroller.h"

#include <QObject>

class PlasmaRecipe : public QObject {
    Q_OBJECT

public:
    PlasmaRecipe(PlasmaController* CTL, QObject* parent = nullptr);
    ~PlasmaRecipe();

    void setRecipeFromFile();
    void processRecipeKeys();

    void setMFCsActualFlow();
    void setRFSetpoint();
    void setTunerSetpoint();
    void setAutoTuneOn();

    QMap<QString, QVariant> getRecipeMap();
    QList<QString> getCascadeRecipeList();

    void addRecipeToCascade(const QString& recipeName);
    void removeRecipeFromCascade(const QString& recipeName);

    void executeCurrentRecipe();

    bool isRecipeComplete();

    void setAutoScanFlag(bool toggle) { m_autoScanFlag = toggle; }
    bool getAutoScanFlag() const { return m_autoScanFlag; }

    void setAutoScan(bool toggle) { m_autoScan = toggle; }
    QString getAutoScan() const { return QString::number(m_autoScan); }
    bool getAutoScanBool() const { return m_autoScan; }

    void setPurge(bool toggle) { m_N2PurgeRecipe = toggle; }
    bool getPurge() const { return m_N2PurgeRecipe; }
    QString getPurgeQStr() const { return QString::number(m_N2PurgeRecipe); }

    void setCycles(int cycles) { m_cycles = cycles; }
    QString getCycles() const { return QString::number(m_cycles); }
    int getCyclesInt() const { return m_cycles; }

    void setSpeed(double speed) { m_speed = speed; }
    QString getSpeed() const { return QString::number(m_speed); }

    void setOverlap(double overlap) { m_overlap = overlap; }
    QString getOverlap() const { return QString::number(m_overlap); }

    void setGap(double gap) { m_gap = gap; }
    QString getGap() const { return QString::number(m_gap); }

    void setThickness(double thickness) { m_thickness = thickness; }
    QString getThickness() const { return QString::number(m_thickness); }

    void setXmin(double xmin) { m_xMin = xmin; }
    QString getXmin() const { return QString::number(m_xMin); }

    void setXmax(double xmax) { m_xMax = xmax; }
    QString getXmax() const { return QString::number(m_xMax); }

    void setYmin(double ymin) { m_yMin = ymin; }
    QString getYmin() const { return QString::number(m_yMin); }

    void setYmax(double ymax) { m_yMax = ymax; }
    QString getYmax() const { return QString::number(m_yMax); }

    FileReader fileReader;

private:
    PlasmaController* CTL_;
    QMap<QString, QVariant> m_recipeMap;
    QList<QString> m_cascadeRecipeList;
    int m_currentRecipeIndex;

    bool m_autoScanFlag;
    bool m_autoScan;
    bool m_N2PurgeRecipe;
    int m_cycles;
    double m_speed;
    double m_overlap;
    double m_gap;
    double m_thickness;
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
};

#endif // PLASMARECIPE_H
