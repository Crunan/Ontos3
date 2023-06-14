#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>

class SerialComms : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString outgoingData READ getOutgoingData WRITE setOutgoingData NOTIFY outgoingDataChanged)
    Q_PROPERTY(QString incomingData READ getIncomingData NOTIFY incomingDataChanged)
    Q_PROPERTY(QString error READ getError NOTIFY errorChanged)

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

    void writeRequest();
    void readData();
    void handleError();

signals:
    void outgoingDataChanged();
    void incomingDataChanged();
    void errorChanged();
};

#endif // SERIALCOMMS_H
