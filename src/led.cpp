#include "include/led.h"

int LED::nextIndex = 0;

LED::LED(const QString& name, int index, LEDType type, bool state)
    : name(name), index(index), type(type), state(state)
{
    if (index == -1) {
        index = nextIndex;
        nextIndex++;
    }

    if (name == "ABORT")
        type = Abort;
    else if (name == "ESTOP")
        type = EStop;
    else if (name == "PL_ON")
        type = PlasmaOn;
    else
        type = Normal;
}

LED::LED(const LED& other)
    : name(other.name), index(other.index), type(other.type), state(other.state)
{
    // No need to copy the static member nextIndex_
}

QString LED::getName() const
{
    return name;
}

void LED::setName(const QString& value)
{
    name = value;
}

int LED::getIndex() const
{
    return index;
}

void LED::setIndex(int value)
{
    index = value;
}

LED::LEDType LED::getType() const
{
    return type;
}

void LED::setType(LEDType value)
{
    type = value;
}

bool LED::getState() const
{
    return state;
}

void LED::setState(bool value)
{
    state = value;
}

void LED::turnOn()
{
    state = true;
}

void LED::turnOff()
{
    state = false;
}

bool LED::isOn() const
{
    return state;
}
