#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <functional>

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

    void setOutgoingData(const QVariant &data);
    void writeOutgoingData();

public:
    SerialComms(QObject *parent = nullptr);
    ~SerialComms();

    QString getOutgoingData() const;
    QString getIncomingData();
    QString getError() const;

    QString prepareCommand(QString cmd, const QString& setpoint);
    using ResponseHandler = std::function<void(const QString&)>;
    void send(const QString& data, ResponseHandler callback = nullptr);

signals:
    void serialPortOpened();
    void serialPortClosed();
    void outgoingDataChanged();
    void incomingDataChanged();
    void errorOccurred();

    void awaitingResponse(ResponseHandler callback = nullptr);

public slots:
    void handleError();
    void openSerialPort(const SettingsDialog::Settings& p);
    void closeSerialPort();
};

#endif // SERIALPORTMANAGER_H
