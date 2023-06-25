#include "include/plasmacontroller/mfc.h"

MFC::MFC(int mfcNumber)
    : QObject(nullptr),
    mfcNumber_(mfcNumber),
    loadedSetpoint_(0.0),
    actualFlow_(0.0),
    range_(0.0)
{

}

QString MFC::getMFCNumberAsString() const
{
    return QString::number(mfcNumber_);
}

int MFC::getMFCNumber() const
{
    return mfcNumber_;
}

QString MFC::getLoadedSetpointAsString() const
{
    return QString::number(loadedSetpoint_);
}

double MFC::getLoadedSetpoint() const
{
    return loadedSetpoint_;
}

void MFC::setLoadedSetpoint(double value)
{
    if (loadedSetpoint_ != value)
    {
        loadedSetpoint_ = value;
        emit setpointChanged();
    }
}

double MFC::getActualFlow() const
{
    return actualFlow_;
}

void MFC::setActualFlow(double value)
{
    if (actualFlow_ != value)
    {
        actualFlow_ = value;
        emit flowChanged(mfcNumber_, actualFlow_);
    }
}

double MFC::getRange() const
{
    return range_;
}

void MFC::setRange(double value)
{
    if (range_ != value)
    {
        range_ = value;
        emit rangeChanged();
    }
}
