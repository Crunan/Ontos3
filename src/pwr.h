#ifndef PWR_H
#define PWR_H

#include <QObject>

class PWR : public QObject {
    Q_OBJECT
    Q_PROPERTY(int recipeWatts READ getRecipeWatts WRITE setRecipeWatts NOTIFY recipeWattsChanged)
    Q_PROPERTY(int forwardWatts READ getForwardWatts WRITE setForwardWatts NOTIFY forwardWattsChanged)
    Q_PROPERTY(int reflectedWatts READ getReflectedWatts WRITE setReflectedWatts NOTIFY reflectedWattsChanged)
    Q_PROPERTY(int defaultRecipe READ getDefaultRecipe WRITE setDefaultRecipe NOTIFY defaultRecipeChanged)

private:
    int recipeWatts_;
    int forwardWatts_;
    int reflectedWatts_;
    int defaultRecipe_;
    int maxWatts_;

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
    void recipeWattsChanged();
    void forwardWattsChanged();
    void reflectedWattsChanged();
    void defaultRecipeChanged();

};

#endif // PWR_H
