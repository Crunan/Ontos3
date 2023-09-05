#ifndef TUNER_H
#define TUNER_H

#include <QObject>

class Tuner : public QObject {

    Q_OBJECT
    Q_PROPERTY(double recipePosition READ getRecipePosition WRITE setRecipePosition NOTIFY recipePositionChanged)
    Q_PROPERTY(double actualPosition READ getActualPosition WRITE setActualPosition NOTIFY actualPositionChanged)
    Q_PROPERTY(double defaultRecipe READ getDefaultRecipe WRITE setDefaultRecipe NOTIFY defaultRecipeChanged)
    Q_PROPERTY(bool autoTune READ getAutoTune WRITE setAutoTune NOTIFY autoTuneChanged)


private:
    double actualPosition_;
    double recipePosition_;
    double defaultRecipe_;
    bool autoTune_;

public:
    Tuner();

    double getActualPosition() const;
    void setActualPosition(double position);

    double getRecipePosition() const;
    void setRecipePosition(double setpoint);

    double getDefaultRecipe() const;
    void setDefaultRecipe(double setpoint);

    bool getAutoTune() const;
    void setAutoTune(bool state);
    void toggleAutoTune();

signals:
    void recipePositionChanged(const double recipePosition);
    void actualPositionChanged(const double actualPosition);
    void defaultRecipeChanged(const double defaultRecipe);
    void autoTuneChanged(const bool autoTune);
};

#endif // TUNER_H
