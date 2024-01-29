#ifndef TUNER_H
#define TUNER_H

#include <QObject>

class Tuner : public QObject {

    Q_OBJECT

public:
    Tuner();

    int getActualPosition() const;
    void setActualPosition(double position);

    int getRecipePosition() const;
    QString getRecipePositionQStr() const;
    void setRecipePosition(double setpoint);// sends value to the controller
    void updateRecipePosition(double setpoint);// updates the ui recipe value

    double getDefaultRecipe() const;
    void setDefaultRecipe(double setpoint);

    bool getAutoTune() const;
    void setAutoTune(bool state);
    void toggleAutoTune();
    void updateAutoTune(bool state);

signals:
    void recipePositionChanged(const double recipePosition);// sends value to the controller
    void updateUIRecipePosition(const double m_recipePosition);// updates the ui recipe value
    void actualPositionChanged(const double actualPosition);
    void defaultRecipeChanged(const double defaultRecipe);
    void autoTuneChanged(const bool autoTune);
    void updateUIAutoTune(const bool m_autoTune);

private:
    double m_actualPosition;
    double m_recipePosition;
    double m_defaultRecipe;
    bool m_autoTune;
};

#endif // TUNER_H
