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
        AC_BAD_HELIUM   = 7,
        AC_ESTOP        = 8,
        AC_DOORS_OPEN   = 9,
        AC_PWR_FWD_LOW  = 10,
        AC_OVER_TEMP    = 11,
        AC_NO_CDA       = 12,
    };

// rev 13?  No idea....
//    Public Enum AbortCode
//        AC_OK = &H0
//              AC_NO_N2 = &H1
//              AC_NO_HEARTBEAT = &H2
//              AC_NO_GAS_1 = &H3
//              AC_NO_GAS_2 = &H4
//              AC_NO_GAS_3 = &H5
//              AC_NO_GAS_4 = &H6
//              AC_BAD_HELIUM = &H7
//              AC_ESTOP = &H8
//              AC_DOORS_OPEN = &H9
//              AC_PWR_FWD_LOW = &HA
//              AC_OVER_TEMP = &HB
//              AC_NO_CDA = &HC
//              End Enum

    explicit AbortCodeMessages(QObject *parent = nullptr);

    QString getAbortMessage(int val);
    bool containsAbortMessage(int val);

signals:
    void showAbortMessageBox(QString abortMessage);

private:
    QMap<AbortCode, QString> m_abortMessages;

};

#endif // ABORTCODEMESSAGES_H
