#include "include/ledstatus.h"

LEDStatus::LEDStatus(QObject *parent)
    : QObject(parent),
    LEDS(),
    _statusBits(0),
    _statusBitsWas(0),
    _abortLed("Abort LED", 0, LED::Abort),
    _estopLed("EStop LED", 1, LED::EStop),
    _plasmaOnLed("PlasmaOn LED", 2, LED::PlasmaOn)
{}

LEDStatus::~LEDStatus() {}

int LEDStatus::getStatusBits() const {
    return _statusBits;
}

void LEDStatus::setStatusBits(int statusBits) {
    if (_statusBits != statusBits) {
        _statusBitsWas = _statusBits;
        _statusBits = statusBits;
        emit statusBitsChanged();
    }
}

int LEDStatus::getStatusBitsWas() const {
    return _statusBitsWas;
}

bool LEDStatus::isAbortActive() const {
    return _abortLed.isOn();
}

bool LEDStatus::isPlasmaActive() const {
    return _plasmaOnLed.isOn();
}

bool LEDStatus::isEstopActive() const {
    return _estopLed.isOn();
}


void LEDStatus::syncLEDStatusFromBits()
{
    for (LED& led : LEDS.ledList) {
        int bitPosition = 1 << led.index;
        if ((_statusBits & bitPosition) > 0) {
            led.turnOn();
            // maybe need to add type setting here.
        } else {
            led.turnOff();
        }
    }
}

