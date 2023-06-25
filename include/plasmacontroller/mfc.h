#ifndef MFC_H
#define MFC_H

#include <QObject>
#include <QProgressBar>

class MFC : public QObject {

    Q_OBJECT
    Q_PROPERTY(double loadedSetpoint READ getLoadedSetpoint WRITE setLoadedSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(double actualFlow READ getActualFlow WRITE setActualFlow NOTIFY flowChanged)
    Q_PROPERTY(double range READ getRange WRITE setRange NOTIFY rangeChanged)

private:
    int mfcNumber_;
    double loadedSetpoint_;  // Private member variable for loadedSetpoint
    double actualFlow_;  // Private member variable for actualFlow
    double range_;

public:
    MFC(int mfcNumber);

    QString getMFCNumberAsString() const;
    int getMFCNumber() const;

    QString getLoadedSetpointAsString() const;
    double getLoadedSetpoint() const;
    void setLoadedSetpoint(double value);

    double getActualFlow() const;
    void setActualFlow(double value);

    double getRange() const;
    void setRange(double value);

signals:
    void setpointChanged();
    void flowChanged(const int mfcNumber, double flow);
    void rangeChanged();

};

#endif // MFC_H
