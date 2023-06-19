#include "include/mainloop.h"


MainLoop::MainLoop(QObject *parent) : QObject(parent)
{
    // Create the event loop
    loop = new QEventLoop(this);

    // Create the timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainLoop::handleTimer);

    // Configure the timer interval (default: 1 second
    setTimerInterval(100);
}

MainLoop::~MainLoop() {
    // Clean up resources
    delete loop;
    delete timer;
}

void MainLoop::start() {
    //Stop the event loop
    loop->exec();
}

void MainLoop::stop() {
    loop->quit();
}

void MainLoop::setTimerInterval(int interval) {
    timer->setInterval(interval);
}



void MainLoop::handleTimer() {
    // Perform actions here on time timeout
    // Emit custom signals if needed!
    // mostly poll here signal.
}
