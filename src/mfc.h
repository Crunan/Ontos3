#ifndef MFC_H
#define MFC_H

#include <QObject>
#include <QProgressBar>

class MFC : public QObject {

    Q_OBJECT

public:
    MFC(int mfcNum);

    QString getMFCNumberQStr() const;
    int getMFCNumber() const;

    QString getRecipeFlowQStr() const;
    double getRecipeFlow() const;
    void setRecipeFlow(double value); // sends value to the controller
    void updateRecipeFlow(double value); // updates the displayed recipe value only

    double getActualFlow() const;
    void setActualFlow(double value); // updates the displayed actual value

    double getRange() const;
    QString getRangeQString() const;
    void setRange(double value);

    double getDefaultRecipe() const;
    void setDefaultRecipe(double value);

signals:
    void updateUIRecipeFlow(const int mfcNumber, double recipeFlow); // updates the ui recipe value
    void recipeFlowChanged(const int mfcNumber, double recipeFlow);// updates the controller recipe value
    void updateUIActualFlow(const int mfcNumber, double actualFlow);// updates the ui actual value
    void rangeChanged(const int mfcNumber, double range);
    void defaultRecipeChanged(const int mfcNumber, double recipeFlow);

private:
    int m_mfcNumber;
    double m_recipeFlow;  // Private member variable for loadedSetpoint
    double m_actualFlow;  // Private member variable for actualFlow
    double m_range;
    double m_defaultRecipe; // Recipe loaded on startup

};

#endif // MFC_H
