#include "mfc.h"

MFC::MFC(int mfcNumber)
    : QObject(nullptr),
    m_mfcNumber(mfcNumber),
    m_recipeFlow(0.0),
    m_actualFlow(0.0),
    m_range(0.0),
    m_defaultRecipe(0.0)
{

}

QString MFC::getMFCNumberAsString() const
{
    return QString::number(m_mfcNumber);
}

int MFC::getMFCNumber() const
{
    return m_mfcNumber;
}

QString MFC::getRecipeFlowAsString() const
{
    return QString::number(m_recipeFlow);
}

double MFC::getRecipeFlow() const
{
    return m_recipeFlow;
}

void MFC::setRecipeFlow(double value)
{
    if (m_recipeFlow != value)
    {
        m_recipeFlow = value;
        emit recipeFlowChanged(m_mfcNumber, m_recipeFlow);
    }
}

double MFC::getActualFlow() const
{
    return m_actualFlow;
}

void MFC::setActualFlow(double value)
{
    if (m_actualFlow != value)
    {
        m_actualFlow = value;
        emit actualFlowChanged(m_mfcNumber, m_actualFlow);
    }
}

double MFC::getRange() const
{
    return m_range;
}

void MFC::setRange(double value)
{
    if (m_range != value)
    {
        m_range = value;
        emit rangeChanged(m_mfcNumber, m_range);    }
}

double MFC::getDefaultRecipe() const
{
    return m_defaultRecipe;
}

void MFC::setDefaultRecipe(double value)
{
    if (m_defaultRecipe!= value)
    {
        m_defaultRecipe = value;
        emit defaultRecipeChanged(m_mfcNumber, m_defaultRecipe);    }
}
