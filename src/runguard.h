/*******************************************************************************
********************************************************************************

   runguard.h:   Header file for application single instance protection
                  object.  Code taken from here:
                  https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection

********************************************************************************
*******************************************************************************/
#ifndef RUNGUARD_H
#define RUNGUARD_H


#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>


class runguard
{

public:
    runguard(const QString& key);
    ~runguard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY( runguard )
};


#endif // RUNGUARD_H
