#ifndef ABORTCODEMESSAGES_H
#define ABORTCODEMESSAGES_H

#include <QObject>
#include <QMap>

class AbortCodeMessages : public QObject
{
    Q_OBJECT
public:
    // TODO: Move these to the config file at some point
    // abort code enum
    enum AbortCode {
        AC_OK           = 0,
        AC_NO_N2        = 1,
        AC_NO_HEARTBEAT = 2,
        AC_NO_GAS_1     = 3,
        AC_NO_GAS_2     = 4,
        AC_NO_GAS_3     = 5,
        AC_NO_GAS_4     = 6,
        AC_NO_GAS_5     = 7,
        AC_NO_GAS_6     = 8,
        AC_BAD_HELIUM   = 9,
        AC_ESTOP        = 10,
        AC_DOORS_OPEN   = 11,
        AC_PWR_FWD_LOW  = 12,
        AC_OVER_TEMP    = 13,
        AC_NO_CDA       = 14,
    };

    explicit AbortCodeMessages(QObject *parent = nullptr);

    QString getAbortMessage(int val);
    bool containsAbortMessage(int val);

signals:
    void showAbortMessageBox(QString abortMessage);

private:
    QMap<AbortCode, QString> m_abortMessages;

};

#endif // ABORTCODEMESSAGES_H
