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

signals:
    // Define custom signals here

private slots:
    void handleTimer();

private:
    QEventLoop* loop;
    QTimer* timer;

};

#endif // MAINLOOP_H
