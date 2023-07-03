#include "recipe.h"

Recipe::Recipe(QObject* parent) : QObject(parent) {
    plasmaRecipe_ = new PlasmaRecipe(this);
    scanRecipe_ = new ScanRecipe(this);
}

Recipe::~Recipe() {
    delete plasmaRecipe_;
    delete scanRecipe_;
}

PlasmaRecipe* Recipe::getPlasmaRecipe() const {
    return plasmaRecipe_;
}

ScanRecipe* Recipe::getScanRecipe() const {
    return scanRecipe_;
}
