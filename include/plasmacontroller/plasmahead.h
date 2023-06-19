#ifndef PLASMAHEAD_H
#define PLASMAHEAD_H

#include <QObject>

class PlasmaHead : public QObject {

    double safetyGap;  //Distance between plasma head & chuck when z at max height
    double slitLength; //Plasma head slit length
    double slitWidth;  //Plasma head slit width
    double currentTemp;

public:
    PlasmaHead();

//    double getTemp() const { return currentTemp; }
//    void setTemp(double temp) {}

//    double getSafetyGap() const { return safetyGap; }
//    void setSafetyGap(double gap) {}

//    double getSlitLength() const { return slitLength; }
//    void setSlitLength(double length) {}

//    double getSlitWidth() const { return slitWidth; }
//    void setSlitWidth(double width) {}

};
#endif // PLASMAHEAD_H
