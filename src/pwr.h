#ifndef PWR_H
#define PWR_H

#include <QObject>

class PWR : public QObject {
    Q_OBJECT

public:
    PWR();

    int getRecipeWatts() const;
    QString getRecipeWattsQStr() const;
    void setRecipeWatts(int value); // sends value to the controller
    void updateRecipeWatts(double value); // updates the displayed recipe value only

    int getForwardWatts() const;
    void setForwardWatts(int value);

    int getMaxForwardWatts() const;
    void setMaxForwardWatts(int value);
    QString getMaxForwardWattsQStr() const;

    int getReflectedWatts() const;
    void setReflectedWatts(int value);

    int getDefaultRecipe() const;
    void setDefaultRecipe(int value);

signals:
    void updateUIRecipeWatts(int watts); // updates the ui recipe value
    void recipeWattsChanged(int watts); // updates the controller recipe value
    void forwardWattsChanged(int watts);
    void reflectedWattsChanged(int watts);
    void defaultRecipeChanged();

private:
    int m_recipeWatts;
    int m_forwardWatts;
    int m_reflectedWatts;
    int m_defaultRecipe;
    int m_maxWatts;
};

#endif // PWR_H
