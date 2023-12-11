#ifndef CASCADERECIPELIST_H
#define CASCADERECIPELIST_H

#include <QString>  // Include the necessary header for QString
#include <QList>

class CascadeRecipeList {
public:
    static void setCascadeRecipe(const QList<QString>& recipeList);

    static void addRecipe(const QString& recipeName);
    static void removeRecipe(const QString& recipeName);
    static QList<QString>& getCascadeList();

public slots:
    static QString getRecipeNames();

private:
    static QList<QString> cascadeRecipe;
};

#endif // CASCADERECIPELIST_H
