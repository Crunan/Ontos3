// LEDStatus.h
#ifndef LEDSTATUS_H
#define LEDSTATUS_H

#include <QObject>
#include "include/ledfactory.h"
#include "include/led.h"

class LEDStatus : public QObject {
    Q_OBJECT
    Q_PROPERTY(int statusBits READ getStatusBits WRITE setStatusBits NOTIFY statusBitsChanged)

public:
    explicit LEDStatus(QObject *parent = nullptr);
    ~LEDStatus();

    LEDFactory LEDS;

    int getStatusBits() const;
    int getStatusBitsWas() const;
    void setStatusBits(int statusBits);

    bool isAbortActive() const;
    bool isPlasmaActive() const;
    bool isEstopActive() const;

     void syncLEDStatusFromBits();

signals:
    void statusBitsChanged();

private:
    int _statusBits;
    int _statusBitsWas;
    LED _abortLed;
    LED _estopLed;
    LED _plasmaOnLed;
};

#endif  // LEDSTATUS_H

