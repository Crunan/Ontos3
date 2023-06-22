#ifndef PWR_H
#define PWR_H

#include <QObject>

class PWR : public QObject {
    Q_OBJECT
    Q_PROPERTY(double loadedSetpoint READ getLoadedSetpoint WRITE setLoadedSetpoint NOTIFY loadedSetpointChanged)
    Q_PROPERTY(double forwardWatts READ getForwardWatts NOTIFY forwardWattsChanged)
    Q_PROPERTY(double reflectedWatts READ getReflectedWatts NOTIFY reflectedWattsChanged)

public:
    PWR();

    double getLoadedSetpoint() const;
    void setLoadedSetpoint(double value);

    double getForwardWatts() const;
    double getReflectedWatts() const;

signals:
    void loadedSetpointChanged();
    void forwardWattsChanged();
    void reflectedWattsChanged();

private:
    double loadedSetpoint_;
    double forwardWatts_;
    double reflectedWatts_;
    int maxWatts_;
};

#endif // PWR_H
