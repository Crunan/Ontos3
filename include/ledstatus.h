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
    int m_statusBits;
    int m_statusBitsWas;
    LED m_abortLed;
    LED m_estopLed;
    LED m_plasmaOnLed;
};

#endif  // LEDSTATUS_H

