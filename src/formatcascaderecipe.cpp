#include "formatcascaderecipe.h"

#include <QStringList>
#include <QString>

QStringList FormatCascadeRecipe::Format(QString& recipes) {
    recipes.replace("\n", "");
    return recipes.split(".rcp");
}
