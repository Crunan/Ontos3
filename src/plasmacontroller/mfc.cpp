#include "include/plasmacontroller/mfc.h"

MFC::MFC(QString mfcNumber)
    : QObject(nullptr),
    mfcNumber_(mfcNumber),
    loadedSetpoint_("0.0"),
    actualFlow_("0.0"),
    range_("0.0")
{

}

QString MFC::getMFCNumber() const
{
    return mfcNumber_;
}
QString MFC::getLoadedSetpoint() const
{
    return loadedSetpoint_;
}

void MFC::setLoadedSetpoint(QString value)
{
    if (loadedSetpoint_ != value)
    {
        loadedSetpoint_ = value;
        emit setpointChanged();
    }
}

QString MFC::getActualFlow() const
{
    return actualFlow_;
}

QString MFC::getRange() const
{
    return range_;
}
