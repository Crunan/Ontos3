#ifndef TUNER_H
#define TUNER_H

#include <QObject>

class Tuner : public QObject {

    Q_OBJECT

public:
    Tuner();

    int getActualPosition() const;
    void setActualPosition(int position);

    int getRecipePosition() const;
    QString getRecipePositionQStr() const;
    void setRecipePosition(int setpoint);

    double getDefaultRecipe() const;
    void setDefaultRecipe(double setpoint);

    bool getAutoTune() const;
    void setAutoTune(bool state);
    void toggleAutoTune();

signals:
    void recipePositionChanged(const int recipePosition);
    void actualPositionChanged(const int actualPosition);
    void defaultRecipeChanged(const double defaultRecipe);
    void autoTuneChanged(const bool autoTune);

private:
    int m_actualPosition;
    int m_recipePosition;
    double m_defaultRecipe;
    bool m_autoTune;
};

#endif // TUNER_H
