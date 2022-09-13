#pragma once
#include <QTimer>

class MyTimer : public QObject
{
    Q_OBJECT
public:
    MyTimer();
    QTimer* timer;

public slots:
    void Tick();

    //void GetTimerName() const { return pTimer ? pTimer-> }
};

