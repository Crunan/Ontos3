#ifndef PLASMARECIPE_H
#define PLASMARECIPE_H

#include "filereader.h"
#include <QObject>

class PlasmaRecipe : public QObject {
    Q_OBJECT

public:
    PlasmaRecipe(QObject* parent = nullptr);
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
    bool getAutoScanBool() const { return m_autoScan; }

    void setPurge(bool toggle) { m_N2PurgeRecipe = toggle; }
    bool getPurge() const { return m_N2PurgeRecipe; }

    void setCycles(int cycles) { m_cycles = cycles; }
    QString getCyclesQStr() const { return QString::number(m_cycles); }
    int getCyclesInt() const { return m_cycles; }

    void setSpeed(double speed) { m_speed = speed; }
    double getSpeed() { return m_speed; }
    QString getSpeedQStr() const { return QString::number(m_speed, 'f', 2); }

    void setOverlap(double overlap) { m_overlap = overlap; }
    double getOverlap() { return m_overlap; }
    QString getOverlapQStr() const { return QString::number(m_overlap, 'f', 2); }

    void setGap(double gap) { m_gap = gap; }
    double getGap() { return m_gap; }
    QString getGapQStr() const { return QString::number(m_gap, 'f', 2); }

    void setThickness(double thickness) { m_thickness = thickness; }
    double getThickness() { return m_thickness; }
    QString getThicknessQStr() const { return QString::number(m_thickness, 'f', 2); }

    void setXmin(double xmin) { m_xMin = xmin; }
    double getXmin() { return m_xMin; }
    QString getXminQStr() const { return QString::number(m_xMin, 'f', 2); }

    void setXmax(double xmax) { m_xMax = xmax; }
    double getXmax() { return m_xMax; }
    QString getXmaxQStr() const { return QString::number(m_xMax, 'f', 2); }

    void setYmin(double ymin) { m_yMin = ymin; }
    double getYmin() { return m_yMin; }
    QString getYminQStr() const { return QString::number(m_yMin, 'f', 2); }

    void setYmax(double ymax) { m_yMax = ymax; }
    double getYmax() { return m_yMax; }
    QString getYmaxQStr() const { return QString::number(m_yMax, 'f', 2); }

    FileReader fileReader;

private:
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
