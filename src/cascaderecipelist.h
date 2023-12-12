#ifndef CASCADERECIPELIST_H
#define CASCADERECIPELIST_H

#include <QString>  // Include the necessary header for QString
#include <QStringList>  // Include the necessary header for QString

class CascadeRecipeList {
public:
    static void setCascadeRecipe(const QStringList& recipeList);

    static void addRecipe(const QString& recipeName);
    static void removeRecipe(const QString& recipeName);
    static QStringList& getCascadeList();

public slots:
    static QString getRecipeNames();

private:
    static QStringList cascadeRecipe;
};

#endif // CASCADERECIPELIST_H
