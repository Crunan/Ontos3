#ifndef LEDFACTORY_H
#define LEDFACTORY_H

#include <QList>
#include <QString>
#include "include/led.h"

class LEDFactory {
public:
    LEDFactory();

    void createLED(const QString& name);
    void setGuiLedColor();


    const QList<LED>& getLedList() const;

private:
    QList<LED> ledList_;
    int lastCreatedLedIndex_;
};

#endif // LEDFACTORY_H
