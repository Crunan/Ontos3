#ifndef PWR_H
#define PWR_H

#include <QObject>

class PWR : public QObject {
    Q_OBJECT

public:
    PWR();

    int getRecipeWatts() const;
    void setRecipeWatts(int value);

    int getForwardWatts() const;
    void setForwardWatts(int value);

    int getReflectedWatts() const;
    void setReflectedWatts(int value);

    int getDefaultRecipe() const;
    void setDefaultRecipe(int value);

signals:
    void recipeWattsChanged(int watts);
    void forwardWattsChanged();
    void reflectedWattsChanged();
    void defaultRecipeChanged();

private:
    int m_recipeWatts;
    int m_forwardWatts;
    int m_reflectedWatts;
    int m_defaultRecipe;
    int m_maxWatts;

};

#endif // PWR_H
