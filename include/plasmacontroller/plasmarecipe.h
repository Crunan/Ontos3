#ifndef PLASMARECIPE_H
#define PLASMARECIPE_H

#include "include/filereader.h"
#include "include/plasmacontroller/plasmacontroller.h"

#include <QObject>

class PlasmaRecipe : public QObject {
    Q_OBJECT

public:
    PlasmaRecipe(PlasmaController* CTL, QObject* parent = nullptr);
    ~PlasmaRecipe();

    void setRecipeFromFile();

    void setMFCsActualFlow();
    void setRFSetpoint();
    void setTunerSetpoint();
    void setAutoTuneOn();
private:
    PlasmaController* CTL_;
    QMap<QString, QVariant> recipeMap_;

public:
    FileReader fileReader;
    void processRecipeKeys();
};

#endif // PLASMARECIPE_H
