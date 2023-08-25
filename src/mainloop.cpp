#include "include/mainloop.h"


MainLoop::MainLoop(QObject *parent) : QObject(parent)
{
    // Create the event loop
    m_pLoop = new QEventLoop(this);

    // Create the timer
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &MainLoop::handleTimer);

    // Configure the timer interval
    setTimerInterval(200);
}

MainLoop::~MainLoop() {
    // Clean up resources
    delete m_pLoop;
    delete m_pTimer;
}

void MainLoop::start() {
    // Start the timer
    m_pTimer->start();

    // Start the event loop
    m_pLoop->exec();
}

void MainLoop::stop() {
    m_pTimer->stop();
    m_pLoop->quit();
}

void MainLoop::setTimerInterval(int interval) {
    m_pTimer->setInterval(interval);
}



void MainLoop::handleTimer() {
    // Perform actions here on time timeout
    // Emit custom signals if needed!
    // mostly poll here signal.
    emit runMainStateMachine();

    m_pLoop->exit();
}
