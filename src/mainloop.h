#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <QObject>
#include <QTimer>
#include <QEventLoop>

class MainLoop : public QObject {
    Q_OBJECT

public:
    explicit MainLoop(QObject *parent = nullptr);
    ~MainLoop();

    void start();
    void stop();
    void setTimerInterval(int interval);
    int getTimerInterval() { return m_pTimer->interval(); }

signals:
    // Define custom signals here
    void runMainStateMachine();

private slots:
    void handleTimer();

private:
    QEventLoop* m_pLoop;
    QTimer* m_pTimer;

};

#endif // MAINLOOP_H
