#ifndef MFC_H
#define MFC_H

#include <QObject>

class MFC : public QObject {

    Q_OBJECT
    Q_PROPERTY(int identifier READ getIdentifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(double loadedSetpoint READ getLoadedSetpoint WRITE setLoadedSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(double actualFlow READ getActualFlow NOTIFY flowChanged)
    Q_PROPERTY(double range READ getRange CONSTANT)

private:
    int identifier_;  // Private member variable for identifier
    double loadedSetpoint_;  // Private member variable for loadedSetpoint
    double actualFlow_;  // Private member variable for actualFlow
    double range_;

public:
    MFC(int number);

    int getIdentifier() const;
    void setIdentifier(int number);

    double getLoadedSetpoint() const;
    void setLoadedSetpoint(double value);

    double getActualFlow() const;
    double getRange() const;

signals:
    void setpointCommandReceived(double setpoint);

    void setpointChanged();
    void flowChanged();
    void identifierChanged();

public slots:

};

#endif // MFC_H
