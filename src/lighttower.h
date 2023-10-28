#ifndef LIGHTTOWER_H
#define LIGHTTOWER_H

#include <QObject>

struct LightTower : public QObject {

    Q_OBJECT

public:
    //Light Tower states
    enum LightState {
        LT_INACTIVE,
        LT_ERROR,
        LT_READY,
        LT_ACTIVE
    };

    LightTower();

    LightState getState() const;
    void setState(LightState state);

signals:
    void lightTowerStateChanged();

private:
    LightState currentState;

};

#endif // LIGHTTOWER_H
