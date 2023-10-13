#ifndef TUNER_H
#define TUNER_H

#include <QObject>

class Tuner : public QObject {

    Q_OBJECT

public:
    Tuner();

    double getActualPosition() const;
    void setActualPosition(double position);

    double getRecipePosition() const;
    QString getRecipePositionQStr() const;
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

private:
    double m_actualPosition;
    double m_recipePosition;
    double m_defaultRecipe;
    bool m_autoTune;
};

#endif // TUNER_H
