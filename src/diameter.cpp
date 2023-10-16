#include "diameter.h"

const int NUM_WAFER_SIZES = 5;

Diameter::Diameter()
{
    m_pWaferDiameter = new int[NUM_WAFER_SIZES] {50, 100, 150, 200, 300};
    m_WaferDiameterText = (QStringList() << "50mm" << "100mm" << "150mm" << "200mm" << "300mm");
}

Diameter::~Diameter()
{
    delete m_pWaferDiameter;
}

int Diameter::getNumWaferSizes() const
{
    return NUM_WAFER_SIZES;
}
