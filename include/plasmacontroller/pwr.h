#ifndef PWR_H
#define PWR_H

#include <QObject>

class PWR : public QObject {
    Q_OBJECT
    Q_PROPERTY(double recipeWatts READ getRecipeWatts WRITE setRecipeWatts NOTIFY recipeWattsChanged)
    Q_PROPERTY(double forwardWatts READ getForwardWatts WRITE setForwardWatts NOTIFY forwardWattsChanged)
    Q_PROPERTY(double reflectedWatts READ getReflectedWatts WRITE setReflectedWatts NOTIFY reflectedWattsChanged)
    Q_PROPERTY(double defaultRecipe READ getDefaultRecipe WRITE setDefaultRecipe NOTIFY defaultRecipeChanged)

private:
    double recipeWatts_;
    double forwardWatts_;
    double reflectedWatts_;
    double defaultRecipe_;
    int maxWatts_;

public:
    PWR();

    double getRecipeWatts() const;
    void setRecipeWatts(double value);

    double getForwardWatts() const;
    void setForwardWatts(double value);

    double getReflectedWatts() const;
    void setReflectedWatts(double value);

    double getDefaultRecipe() const;
    void setDefaultRecipe(double value);

signals:
    void recipeWattsChanged();
    void forwardWattsChanged();
    void reflectedWattsChanged();
    void defaultRecipeChanged();

};

#endif // PWR_H
