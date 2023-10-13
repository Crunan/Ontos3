#ifndef PLASMARECIPE_H
#define PLASMARECIPE_H

#include "filereader.h"
#include <QObject>

const QString RECIPE_PWR_KEY = "PWR";
const QString RECIPE_TUNER_KEY = "TUNER";
const QString RECIPE_MFC_KEY = "MFC";
const QString RECIPE_MFC1_KEY = "MFC1";
const QString RECIPE_MFC2_KEY = "MFC2";
const QString RECIPE_MFC3_KEY = "MFC3";
const QString RECIPE_MFC4_KEY = "MFC4";
const QString RECIPE_THICKNESS_KEY = "THICKNESS";
const QString RECIPE_GAP_KEY = "GAP";
const QString RECIPE_OVERLAP_KEY = "OVERLAP";
const QString RECIPE_SPEED_KEY = "SPEED";
const QString RECIPE_CYCLES_KEY = "CYCLES";
const QString RECIPE_XMIN_KEY = "XMIN";
const QString RECIPE_YMIN_KEY = "YMIN";
const QString RECIPE_XMAX_KEY = "XMAX";
const QString RECIPE_YMAX_KEY = "YMAX";
const QString RECIPE_PURGE_KEY = "PURGE";
const QString RECIPE_AUTOSCAN_KEY = "AUTOSCAN";
const QString RECIPE_HEATER_KEY = "HEATER";

class PlasmaRecipe : public QObject {
    Q_OBJECT

public:
    PlasmaRecipe(QObject* parent = nullptr);
    ~PlasmaRecipe();

    void setRecipeFromFile();
    void processRecipeKeys();

    QMap<QString, QVariant> getRecipeMap();
    QList<QString> getCascadeRecipeList();

    void addRecipeToCascade(const QString& recipeName);
    void removeRecipeFromCascade(const QString& recipeName);

    void executeCurrentRecipe();

    bool isRecipeComplete();

    void setThicknessFromRecipe();
    void setGapFromRecipe();
    void setOverlapFromRecipe();
    void setSpeedFromRecipe();
    void setAutoScanFromRecipe();
    void setXaxisLimitsFromRecipe();
    void setYaxisLimitsFromRecipe();
    void setPurgeFromRecipe();
    void setHeaterFromRecipe();
    void setCyclesFromRecipe();

    void setAutoScanFlag(bool toggle);
    bool getAutoScanFlag() const { return m_autoScanFlag; }

    void setAutoScan(bool toggle);
    bool getAutoScanBool() const { return m_autoScan; }
    QString getAutoScanQStr() const { return QString::number(m_autoScan);  }

    void setPurge(bool toggle);
    bool getPurge() const { return m_N2PurgeRecipe; }
    QString getPurgeQStr() const { return QString::number(m_N2PurgeRecipe); }

    void setHeater(bool heater);
    bool getHeater() const { return m_heater; }
    QString getHeaterQStr() const { return QString::number(m_heater);; }

    void setCycles(int cycles);
    QString getCyclesQStr() const { return QString::number(m_cycles); }
    int getCyclesInt() const { return m_cycles; }

    void setSpeed(double speed);
    double getSpeed() { return m_speed; }
    QString getSpeedQStr() const { return QString::number(m_speed, 'f', 2); }

    void setOverlap(double overlap);
    double getOverlap() { return m_overlap; }
    QString getOverlapQStr() const { return QString::number(m_overlap, 'f', 2); }

    void setGap(double gap);
    double getGap() { return m_gap; }
    QString getGapQStr() const { return QString::number(m_gap, 'f', 2); }

    void setThickness(double thickness);
    double getThickness() { return m_thickness; }
    QString getThicknessQStr() const { return QString::number(m_thickness, 'f', 2); }

    void setXmin(double xmin);
    double getXmin() { return m_xMin; }
    QString getXminQStr() const { return QString::number(m_xMin, 'f', 2); }

    void setXmax(double xmax);
    double getXmax() { return m_xMax; }
    QString getXmaxQStr() const { return QString::number(m_xMax, 'f', 2); }

    void setYmin(double ymin);
    double getYmin() { return m_yMin; }
    QString getYminQStr() const { return QString::number(m_yMin, 'f', 2); }

    void setYmax(double ymax);
    double getYmax() { return m_yMax; }
    QString getYmaxQStr() const { return QString::number(m_yMax, 'f', 2); }

    FileReader fileReader;

signals:
    void thicknessChanged();
    void gapChanged();
    void overlapChanged();
    void speedChanged();
    void autoScanChanged();
    void xLimitsChanged();
    void yLimitsChanged();
    void cyclesChanged();

private:
    QMap<QString, QVariant> m_recipeMap;
    QList<QString> m_cascadeRecipeList;
    int m_currentRecipeIndex;

    bool m_autoScanFlag;
    bool m_autoScan;
    bool m_N2PurgeRecipe;
    bool m_heater;
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
