#ifndef MANEUVERS_H
#define MANEUVERS_H

#include <QString>
#include <QList>

namespace stage {

class Maneuvers
{
public:
    Maneuvers();

    void move(QString axis, QString speed, QString position);

    void moveSequence(const QList<double>& sequence);

    void stop();

    void emergencyStop();

private:
         // Private member functions and variables

};

} // namespace stage

#endif // MANEUVERS_H
