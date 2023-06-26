#include "include/plasmacontroller/mfc.h"

MFC::MFC(int mfcNumber)
    : QObject(nullptr),
    mfcNumber_(mfcNumber),
    recipeFlow_(0.0),
    actualFlow_(0.0),
    range_(0.0),
    defaultRecipe_(0.0)
{

}

QString MFC::getMFCNumberAsString() const
{
    return QString::number(mfcNumber_);
}

int MFC::getMFCNumber() const
{
    return mfcNumber_;
}

QString MFC::getRecipeFlowAsString() const
{
    return QString::number(recipeFlow_);
}

double MFC::getRecipeFlow() const
{
    return recipeFlow_;
}

void MFC::setRecipeFlow(double value)
{
    if (recipeFlow_ != value)
    {
        recipeFlow_ = value;
        emit recipeFlowChanged(mfcNumber_, recipeFlow_);
    }
}

double MFC::getActualFlow() const
{
    return actualFlow_;
}

void MFC::setActualFlow(double value)
{
    if (actualFlow_ != value)
    {
        actualFlow_ = value;
        emit actualFlowChanged(mfcNumber_, actualFlow_);
    }
}

double MFC::getRange() const
{
    return range_;
}

void MFC::setRange(double value)
{
    if (range_ != value)
    {
        range_ = value;
        emit rangeChanged(mfcNumber_, range_);    }
}

double MFC::getDefaultRecipe() const
{
    return defaultRecipe_;
}

void MFC::setDefaultRecipe(double value)
{
    if (defaultRecipe_!= value)
    {
        defaultRecipe_ = value;
        emit defaultRecipeChanged(mfcNumber_, defaultRecipe_);    }
}
