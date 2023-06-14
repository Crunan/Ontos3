#ifndef COORDINATE_H
#define COORDINATE_H

#include <QObject>

struct Coordinate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double position READ getPosition WRITE setPosition NOTIFY positionChanged)

private:
    double position;

public:
    double getPosition() const;
    void setPosition(double newPosition);

signals:
    void positionChanged();

};


#endif // COORDINATE_H
