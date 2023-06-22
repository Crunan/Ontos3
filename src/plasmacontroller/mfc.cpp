#include "include/plasmacontroller/mfc.h"

MFC::MFC()
    : identifier_(0), loadedSetpoint_(0.0), actualFlow_(0.0), range_(0.0)
{

}

int MFC::getIdentifier() const
{
    return identifier_;
}

void MFC::setIdentifier(int number)
{
    if (identifier_ != number) {
        identifier_ = number;
        emit identifierChanged();
    }
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
