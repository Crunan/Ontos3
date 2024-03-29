#ifndef PLASMAHEAD_H
#define PLASMAHEAD_H

#include <QObject>

class PlasmaHead : public QObject
{
    Q_OBJECT

public:
    PlasmaHead();
    void setSafetyGap(double safetyGap);
    double getSafetyGap() const;
    double getSlitLength() const;
    void setSlitlength(double slitLength);
    QString getSlitLengthQStr() const;
    double getSlitWidth() const;
    void setSlitWidth(double slitWidth);
    double getTemperature() const;
    void setTemperature(double temperature);

signals:
    void headTemperatureChanged();
    void headSlitLengthChanged();
    void headSlitWidthChanged();

private:
    double m_safetyGap;  // Distance between plasma head & chuck when z at max height
    double m_slitLength; // Plasma head slit length
    double m_slitWidth;  // Plasma head slit width
    double m_temperature;

};

#endif // PLASMAHEAD_H
