#include "formatcascaderecipe.h"

#include <QStringList>
#include <QString>

QStringList FormatCascadeRecipe::Format(QString& recipes) {
    recipes.replace("\n", "");
    QStringList recipeList = recipes.split(".rcp", Qt::SkipEmptyParts);

    // Append ".rcp" to each item
    for (QString& recipe : recipeList) {
        recipe.append(".rcp");
    }

    return recipeList;
}
