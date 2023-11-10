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
    void setRecipePosition(int setpoint);// sends value to the controller
    void updateRecipePosition(int setpoint);// updates the ui recipe value

    double getDefaultRecipe() const;
    void setDefaultRecipe(double setpoint);

    bool getAutoTune() const;
    void setAutoTune(bool state);
    void toggleAutoTune();
    void updateAutoTune(bool state);

signals:
    void recipePositionChanged(const int recipePosition);// sends value to the controller
    void updateUIRecipePosition(const int m_recipePosition);// updates the ui recipe value
    void actualPositionChanged(const int actualPosition);
    void defaultRecipeChanged(const double defaultRecipe);
    void autoTuneChanged(const bool autoTune);
    void updateUIAutoTune(const bool m_autoTune);

private:
    int m_actualPosition;
    int m_recipePosition;
    double m_defaultRecipe;
    bool m_autoTune;
};

#endif // TUNER_H
