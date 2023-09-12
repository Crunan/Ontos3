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
private:
    PlasmaController* CTL_;
    QMap<QString, QVariant> m_recipeMap;
    QList<QString> cascadeRecipeList_;
    int m_currentRecipeIndex;

public:
    FileReader fileReader;
};

#endif // PLASMARECIPE_H
