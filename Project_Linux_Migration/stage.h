#ifndef STAGE_H
#define STAGE_H

#include "coordinate.h"

struct Stage {
private:
    // Coordinates relative to stage base
    Coordinate stageBase;
    Coordinate pinsBuried;  // Distance in Z from reset-0 to bury the lift pins
    Coordinate pinsExposed; // Distance in Z from reset-0 to expose the lift pins

public:
    Stage();

    double getStageBase() const;
    void setStageBase(double newPos);

    double getPinsBuried() const;
    void setPinsBuried(double newPos);

    double getPinsExposed() const;
    void setPinsExposed(double newPos);
};

#endif // STAGE_H
