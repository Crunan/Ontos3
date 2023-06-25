#ifndef MFC_H
#define MFC_H

#include <QObject>

class MFC : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString loadedSetpoint READ getLoadedSetpoint WRITE setLoadedSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(QString actualFlow READ getActualFlow NOTIFY flowChanged)
    Q_PROPERTY(QString range READ getRange CONSTANT)

private:
    QString mfcNumber_;
    QString loadedSetpoint_;  // Private member variable for loadedSetpoint
    QString actualFlow_;  // Private member variable for actualFlow
    QString range_;

public:
    MFC(QString mfcNumber);

    QString getMFCNumber() const;
    QString getLoadedSetpoint() const;
    void setLoadedSetpoint(QString value);

    QString getActualFlow() const;
    QString getRange() const;

signals:   
    void setpointChanged();
    void flowChanged();
};

#endif // MFC_H
