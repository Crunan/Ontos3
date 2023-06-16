#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>

class SerialComms : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString outgoingData READ getOutgoingData CONSTANT WRITE setOutgoingData NOTIFY outgoingDataChanged)
    Q_PROPERTY(QString incomingData READ getIncomingData CONSTANT NOTIFY incomingDataChanged)
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
    void setOutgoingData(const QString &data);

    QString getIncomingData() const;

    QString getError() const;


    void handleError();
    void setSerialSettings();

signals:
    void serialPortOpened();
    void outgoingDataChanged();
    void incomingDataChanged();
    void errorOccured();
};

#endif // SERIALCOMMS_H
