#include "mfc.h"

MFC::MFC()
    : loadedSetpoint(0.0), actualFlow(0.0), range(0.0)
{

}

double MFC::getSetpoint() const
{
    return loadedSetpoint;
}

void MFC::setSetpoint(double value)
{
    if (loadedSetpoint != value)
    {
        loadedSetpoint = value;
        emit setpointChanged();
    }
}

double MFC::getRange() const
{
    return range;
}

double MFC::getFlow() const
{
    return actualFlow;
}
