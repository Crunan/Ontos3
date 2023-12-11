// CascadeRecipeList.cpp

#include "cascaderecipelist.h"

QList<QString> CascadeRecipeList::cascadeRecipe;  // Don't forget to define the static member

void CascadeRecipeList::setCascadeRecipe(const QList<QString> &recipeList) {
    cascadeRecipe = recipeList;
}

void CascadeRecipeList::addRecipe(const QString& recipeName) {
    cascadeRecipe.append(recipeName);
}

void CascadeRecipeList::removeRecipe(const QString& recipeName) {
    cascadeRecipe.removeOne(recipeName);
}

QList<QString>& CascadeRecipeList::getCascadeList() {
    return cascadeRecipe;
}
QString CascadeRecipeList::getRecipeNames() {
    QString recipes;

    for (const QString& recipe : cascadeRecipe) {
        recipes += recipe;
    }

    return recipes;
}

