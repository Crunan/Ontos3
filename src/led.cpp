#include "include/led.h"

int LED::nextIndex_ = 0;

LED::LED(const QString& name, int index, LEDType type, bool state)
    : name_(name), index_(index), type_(type), state_(state)
{
    if (index_ == -1) {
        index_ = nextIndex_;
        nextIndex_++;
    }

    if (name == "ABORT")
        type_ = Abort;
    else if (name == "ESTOP")
        type_ = EStop;
    else if (name == "PL_ON")
        type_ = PlasmaOn;
    else
        type_ = Normal;
}

QString LED::getName() const
{
    return name_;
}

void LED::setName(const QString& name)
{
    name_ = name;
}

int LED::getIndex() const
{
    return index_;
}

void LED::setIndex(int index)
{
    index_ = index;
}

LED::LEDType LED::getType() const
{
    return type_;
}

void LED::setType(LEDType type)
{
    type_ = type;
}

bool LED::getState() const
{
    return state_;
}

void LED::setState(bool state)
{
    state_ = state;
}

void LED::turnOn()
{
    state_ = true;
}

void LED::turnOff()
{
    state_ = false;
}

bool LED::isOn() const
{
    return state_;
}
