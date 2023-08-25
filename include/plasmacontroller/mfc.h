#ifndef MFC_H
#define MFC_H

#include <QObject>
#include <QProgressBar>

class MFC : public QObject {

    Q_OBJECT

private:
    int mfcNumber_;
    double recipeFlow_;  // Private member variable for loadedSetpoint
    double actualFlow_;  // Private member variable for actualFlow
    double range_;
    double defaultRecipe_; // Recipe loaded on startup

public:
    MFC(int mfcNum);

    QString getMFCNumberAsString() const;
    int getMFCNumber() const;

    QString getRecipeFlowAsString() const;
    double getRecipeFlow() const;
    void setRecipeFlow(double value);

    double getActualFlow() const;
    void setActualFlow(double value);

    double getRange() const;
    void setRange(double value);

    double getDefaultRecipe() const;
    void setDefaultRecipe(double value);

signals:
    void recipeFlowChanged(const int mfcNumber, double recipeFlow);
    void actualFlowChanged(const int mfcNumber, double actualFlow);
    void rangeChanged(const int mfcNumber, double range);
    void defaultRecipeChanged(const int mfcNumber, double recipeFlow);

};

#endif // MFC_H
