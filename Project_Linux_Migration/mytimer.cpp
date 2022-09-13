#include "mytimer.h"
#include <QDebug>

MyTimer::MyTimer()
{
    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(Tick()));

    // msec
    timer->start(1000);
}

void MyTimer::Tick()
{
    qDebug() << "1000 (ms) Interval...";
}
