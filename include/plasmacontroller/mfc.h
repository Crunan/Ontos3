#ifndef MFC_H
#define MFC_H

#include <QObject>

class MFC : public QObject {

    Q_OBJECT  
    Q_PROPERTY(double loadedSetpoint READ getLoadedSetpoint WRITE setLoadedSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(double actualFlow READ getActualFlow NOTIFY flowChanged)
    Q_PROPERTY(double range READ getRange CONSTANT)

private:
    double loadedSetpoint_;  // Private member variable for loadedSetpoint
    double actualFlow_;  // Private member variable for actualFlow
    double range_;

public:
    MFC();

    double getLoadedSetpoint() const;
    void setLoadedSetpoint(double value);

    double getActualFlow() const;
    double getRange() const;

signals:
    void setpointCommandReceived(double setpoint);

    void setpointChanged();
    void flowChanged();

public slots:

};

#endif // MFC_H
