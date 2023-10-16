#ifndef STAGE_H
#define STAGE_H

#include <QObject>

class Stage : public QObject
{
    Q_OBJECT
public:
    explicit Stage(QObject *parent = nullptr);

    void setPinsBuried(double pos) { pinsBuriedPos = pos; }
    void setPinsExposed(double pos) { pinsExposedPos = pos; }

    double getPinsBuriedPos() { return pinsBuriedPos; }
    double getPinsExposedPos() { return pinsExposedPos; }

    QString getPinsBuriedPosQStr() {  return QString::number(pinsBuriedPos); }
    QString getPinsExposedPosQStr() { return QString::number(pinsExposedPos); }

signals:

private:
    double pinsBuriedPos;  //Distance in Z from reset-0 to bury the lift pins
    double pinsExposedPos; //Distance in Z from reset-0 to expose the lift pins
};

#endif // STAGE_H
