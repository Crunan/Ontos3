#include "formatcascaderecipe.h"

#include <QStringList>
#include <QString>

QList<QString> FormatCascadeRecipe::Format(QString& recipes) {
    QList<QString> recipeList;

    // Split the input QString using the delimiter ".rcp"
    QStringList recipeTokens = recipes.split(".rcp", Qt::KeepEmptyParts);

    // Add each recipe to the QList
    for (const QString& recipe : recipeTokens) {
        // Ensure the recipe ends with ".rcp" (add it back if needed)
        QString formattedRecipe = recipe.trimmed();
        if (!formattedRecipe.endsWith(".rcp")) {
            formattedRecipe += ".rcp";
        }

        recipeList.append(formattedRecipe);
    }

    return recipeList;
}
