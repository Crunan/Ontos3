// CascadeRecipeList.cpp

#include "cascaderecipelist.h"

QStringList CascadeRecipeList::cascadeRecipe;  // Don't forget to define the static member

void CascadeRecipeList::setCascadeRecipe(const QStringList& recipeList) {
    cascadeRecipe = recipeList;
}

void CascadeRecipeList::addRecipe(const QString& recipeName) {
    cascadeRecipe.append(recipeName);
}

void CascadeRecipeList::removeRecipe(const QString& recipeName) {
    cascadeRecipe.removeOne(recipeName);
}

QStringList& CascadeRecipeList::getCascadeList() {
    return cascadeRecipe;
}
QString CascadeRecipeList::getRecipeNames() {
    QString recipes;

    for (const QString& recipe : cascadeRecipe) {
        recipes += recipe;
    }

    return recipes;
}

