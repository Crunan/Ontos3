#include "mfc.h"

MFC::MFC(int mfcNumber)
    : QObject(nullptr),
    m_mfcNumber(mfcNumber),
    m_recipeFlow(0.0),
    m_actualFlow(0.0),
    m_range(0.0),
    m_defaultRecipe(0.0)
{}

QString MFC::getMFCNumberQStr() const
{
    return QString::number(m_mfcNumber);
}

int MFC::getMFCNumber() const
{
    return m_mfcNumber;
}

QString MFC::getRecipeFlowQStr() const
{
    return QString::number(m_recipeFlow, 'f', 2);
}

double MFC::getRecipeFlow() const
{
    return m_recipeFlow;
}


void MFC::setRecipeFlow(double value)
{
    m_recipeFlow = value;
    emit recipeFlowChanged(m_mfcNumber, m_recipeFlow); // update the controller
}

void MFC::updateRecipeFlow(double value)
{
    m_recipeFlow = value;
    emit updateUIRecipeFlow(m_mfcNumber, m_recipeFlow); // updates the UI recipe flow
}

double MFC::getActualFlow() const
{
    return m_actualFlow;
}

void MFC::setActualFlow(double value)
{
    if (m_actualFlow != value) {
        m_actualFlow = value;
        emit updateUIActualFlow(m_mfcNumber, m_actualFlow); // updates the UI actual flow
    }
}

double MFC::getRange() const
{
    return m_range;
}

void MFC::setRange(double value)
{
    m_range = value;
    emit rangeChanged(m_mfcNumber, m_range);
}

double MFC::getDefaultRecipe() const
{
    return m_defaultRecipe;
}

void MFC::setDefaultRecipe(double value)
{
    m_defaultRecipe = value;
    emit defaultRecipeChanged(m_mfcNumber, m_defaultRecipe);
}
