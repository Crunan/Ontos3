#include "ledstatus.h"

LEDStatus::LEDStatus(QObject *parent)
    : QObject(parent),
    LEDS(),
    m_statusBits(0),
    m_statusBitsWas(0),
    m_abortLed("Abort LED", 0, LED::Abort),
    m_estopLed("EStop LED", 1, LED::EStop),
    m_plasmaOnLed("PlasmaOn LED", 2, LED::PlasmaOn)
{}

LEDStatus::~LEDStatus() {}

int LEDStatus::getStatusBits() const {
    return m_statusBits;
}

void LEDStatus::setStatusBits(int statusBits) {
    syncLEDStatusFromBits();
    if (m_statusBits != statusBits) {
        m_statusBitsWas = m_statusBits;
        m_statusBits = statusBits;

        emit statusBitsChanged();
    }
}

int LEDStatus::getStatusBitsWas() const {
    return m_statusBitsWas;
}

bool LEDStatus::isAbortActive() const {
    return m_abortLed.isOn();
}

bool LEDStatus::isPlasmaActive() const {
    return m_plasmaOnLed.isOn();
}

bool LEDStatus::isEstopActive() const {
    return m_estopLed.isOn();
}


void LEDStatus::syncLEDStatusFromBits()
{
    if (m_abortLed.isOn())
        m_abortLed.turnOff();
    else
        m_abortLed.turnOn();

//    for (LED& led : LEDS.ledList) {
//        int bitPosition = 1 << led.index;
//        if ((m_statusBits & bitPosition) > 0) {
//            led.turnOn();
//            // maybe need to add type setting here.
//        } else {
//            led.turnOff();
//        }
//    }
}

