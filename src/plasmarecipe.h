#ifndef PLASMARECIPE_H
#define PLASMARECIPE_H

#include "filedescriptor.h"
#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringLiteral>

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
    int getNumCascadeRecipes() { return m_cascadeRecipeList.length(); }
    void resetCascadeIndex() { m_currentRecipeIndex = 0; }
    void incrementCascadeIndex() { m_currentRecipeIndex++; }
    int getCurentCascadeIndex() { return m_currentRecipeIndex; }
    void clearCascadeRecipes();
    void executeCurrentRecipe();

    void setAutoScan(bool toggle);
    bool getAutoScanBool() const;
    QString getAutoScanQStr() const;

    void setPurge(bool toggle);
    bool getPurge() const;
    QString getPurgeQStr() const;

    void setHeater(bool heater);
    bool getHeater() const;
    QString getHeaterQStr() const;

    void setCycles(int cycles);
    QString getCyclesQStr() const;
    int getCyclesInt() const;

    void setSpeed(double speed);
    double getSpeed() const;
    QString getSpeedQStr() const;

    void setOverlap(double overlap);
    double getOverlap() const;
    QString getOverlapQStr() const;

    void setGap(double gap);
    double getGap() const;
    QString getGapQStr() const;

    void setThickness(double thickness);
    double getThickness() const;
    QString getThicknessQStr() const;

    void setXminPH(double xmin);
    double getXminPH() const;
    QString getXminPHQStr() const;

    void setXmaxPH(double xmax);
    double getXmaxPH() const;
    QString getXmaxPHQStr() const;

    void setYminPH(double ymin);
    double getYminPH() const;
    QString getYminPHQStr() const;

    void setYmaxPH(double ymax);
    double getYmaxPH() const;
    QString getYmaxPHQStr() const;

    FileDescriptor fileMetaData;

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
    void initializeMap();

    QMap<QString, QVariant> m_recipeMap;
    QList<QString> m_cascadeRecipeList;
    int m_currentRecipeIndex;
};

#endif // PLASMARECIPE_H
