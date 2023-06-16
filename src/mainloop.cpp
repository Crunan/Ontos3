#include "include/mainloop.h"

#include <QObject>
#include <QTimer>
#include <QEventLoop>

class MainLoop : public QObject
{
    Q_OBJECT

public:
    MainLoop(QObject *parent = nullptr) : QObject(parent)
    {
        // Create the event loop
        loop = new QEventLoop(this);

        // Create the timer
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainLoop::handleTimer);

        // Configure the timer interval (default: 1 second
        setTimerInterval(100);
    }

    ~MainLoop() {
        // Clean up resources
        delete loop;
        delete timer;
    }

    void start() {
        //Stop the event loop
        loop->exec();
    }

    void stop() {
        loop->quit();
    }

    void setTimerInterval(int interval) {
        timer->setInterval(interval);
    }


signals:
    // Define custom signals here

private slots:
    void handleTimer() {
        // Perform actions here on time timeout
        // Emit custom signals if needed!
        // mostly poll here signal.
    }
};
