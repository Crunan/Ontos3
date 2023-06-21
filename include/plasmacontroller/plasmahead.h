#ifndef PLASMAHEAD_H
#define PLASMAHEAD_H

#include <QObject>

class PlasmaHead : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double safetyGap READ getSafetyGap CONSTANT)
    Q_PROPERTY(double slitLength READ getSlitLength CONSTANT)
    Q_PROPERTY(double slitWidth READ getSlitWidth CONSTANT)
    Q_PROPERTY(double temp READ getTemp WRITE setTemp NOTIFY tempChanged)

private:
    double safetyGap;  // Distance between plasma head & chuck when z at max height
    double slitLength; // Plasma head slit length
    double slitWidth;  // Plasma head slit width
    double currentTemp;

public:
    PlasmaHead(double initialSafetyGap, double initialSlitLength, double initialSlitWidth, double initialTemp);

    double getSafetyGap() const;
    double getSlitLength() const;
    double getSlitWidth() const;
    double getTemp() const;
    void setTemp(double temp);

signals:
    void tempChanged();
};

#endif // PLASMAHEAD_H
