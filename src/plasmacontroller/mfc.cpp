#include "include/plasmacontroller/mfc.h"

MFC::MFC()
    : loadedSetpoint_(0.0), actualFlow_(0.0), range_(0.0)
{

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

double MFC::getRange() const
{
    return range_;
}

double MFC::getActualFlow() const
{
    return actualFlow_;
}
