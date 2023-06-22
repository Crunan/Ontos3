#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>

#include "include/settingsdialog.h"

class SerialComms : public QObject
/* This class will handle the communication with the serial
 * device. It should provide methods for sending
 * and receiving data over the serial connection.
 */

{
    Q_OBJECT
    Q_PROPERTY(QString outgoingData READ getOutgoingData  WRITE setOutgoingData NOTIFY outgoingDataChanged)
    Q_PROPERTY(QString incomingData READ getIncomingData NOTIFY incomingDataChanged)
    Q_PROPERTY(QString error READ getError CONSTANT)

private:
    QString outgoingData;
    QString incomingData;
    QString error;
    QSerialPort *serial;

public:
    SerialComms(QObject *parent = nullptr);
    ~SerialComms();

    QString getOutgoingData() const;
    void setOutgoingData(const QVariant &data);

    QString getIncomingData();
    QString getError() const;

    void writeOutgoingData();
signals:
    void serialPortOpened();
    void serialPortClosed();
    void outgoingDataChanged();
    void incomingDataChanged();
    void errorOccurred();

public slots:
    void handleError();
    void openSerialPort(const SettingsDialog::Settings& p);
    void closeSerialPort();
};

#endif // SERIALCOMMS_H
