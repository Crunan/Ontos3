#ifndef LEDFACTORY_H
#define LEDFACTORY_H

#include <QList>
#include <QString>
#include "led.h"

class LEDFactory {
public:
    LEDFactory();

    void createLED(const QString& name);
    void setGuiLedColor();


    const QList<LED>& getLedList() const;

public:
    QList<LED> ledList;
    int lastCreatedLedIndex;
};

#endif // LEDFACTORY_H
