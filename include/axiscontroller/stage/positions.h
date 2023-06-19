#ifndef POSITIONS_H
#define POSITIONS_H

#include "../coordinate.h"

class Positions {
private:
    // Coordinates relative to stage base
    Coordinate base;
    Coordinate pinsBuried;  // Distance in Z from reset-0 to bury the lift pins
    Coordinate pinsExposed; // Distance in Z from reset-0 to expose the lift pins

public:
    Positions();

    double getBase() const;
    void setStageBase(double newPos);

    double getPinsBuried() const;
    void setPinsBuried(double newPos);

    double getPinsExposed() const;
    void setPinsExposed(double newPos);
};

#endif // POSITIONS_H
