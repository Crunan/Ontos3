#ifndef PLASMAHEAD_H
#define PLASMAHEAD_H

#include <QObject>

class PlasmaHead : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double safetyGap READ getSafetyGap CONSTANT)
    Q_PROPERTY(double slitLength READ getSlitLength CONSTANT)
    Q_PROPERTY(double slitWidth READ getSlitWidth CONSTANT)
    Q_PROPERTY(double temperature READ getTemperature WRITE setTemperature NOTIFY temperatureChanged)

private:
    double safetyGap_;  // Distance between plasma head & chuck when z at max height
    double slitLength_; // Plasma head slit length
    double slitWidth_;  // Plasma head slit width
    double temperature_;

public:
    PlasmaHead();

    double getSafetyGap() const;
    double getSlitLength() const;
    double getSlitWidth() const;
    double getTemperature() const;
    void setTemperature(double temperature);

signals:
    void temperatureChanged();

public slots:

};

#endif // PLASMAHEAD_H
