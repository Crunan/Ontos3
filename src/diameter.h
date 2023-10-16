#ifndef DIAMETER_H
#define DIAMETER_H

#include <QStringList>

class Diameter
{
public:
    Diameter();
    ~Diameter();

    int getWaferDiameterByIndex(int index) { return m_pWaferDiameter[index]; }
    QString getWaferDiameterTextByIndex(int index) { return m_WaferDiameterText[index]; }
    QStringList getWaferDiameterTextList() { return m_WaferDiameterText; }

    int getCurrentWaferDiameterSelection() { return m_currentWaferDiameter; }
    void setCurrentWaferDiameter(int waferDiameter) { m_currentWaferDiameter = waferDiameter; }

    int getNumWaferSizes() const;
private:
    int* m_pWaferDiameter;
    QStringList m_WaferDiameterText;

    int m_currentWaferDiameter;
};

#endif // DIAMETER_H
