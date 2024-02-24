#include "abortcodemessages.h"

AbortCodeMessages::AbortCodeMessages(QObject *parent) :
    QObject{parent},
    m_abortMessages()
{
    m_abortMessages[AC_OK] = "";
    m_abortMessages[AC_NO_N2] = "NO PURGE N2";
    m_abortMessages[AC_NO_HEARTBEAT] = "NO HEARTBEAT";
    m_abortMessages[AC_NO_GAS_1] = "MFC_1 Low Flow";
    m_abortMessages[AC_NO_GAS_2] = "MFC_2 Low Flow";
    m_abortMessages[AC_NO_GAS_3] = "MFC_3 Low Flow";
    m_abortMessages[AC_NO_GAS_4] = "MFC_4 Low Flow";
    m_abortMessages[AC_NO_GAS_5] = "MFC_5 Low Flow";
    m_abortMessages[AC_NO_GAS_6] = "MFC_6 Low Flow";
    m_abortMessages[AC_BAD_HELIUM] = "BAD HELIUM";
    m_abortMessages[AC_ESTOP] = "ESTOP ACTIVE";
    m_abortMessages[AC_DOORS_OPEN] = "ABORT: DOOR OPENED, Please close the door and press Acknowledge to reinitialize";
    m_abortMessages[AC_PWR_FWD_LOW] = "Power Fwd Low";
    m_abortMessages[AC_OVER_TEMP] = "Head Too Hot";
    m_abortMessages[AC_NO_CDA] = "No CDA";
}

QString AbortCodeMessages::getAbortMessage(int val)
{
    return m_abortMessages[static_cast<AbortCode>(val)];
}

bool AbortCodeMessages::containsAbortMessage(int val)
{
    bool found = m_abortMessages.contains(static_cast<AbortCode>(val));
    return found;
}

