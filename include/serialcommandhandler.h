#ifndef SERIALCOMMANDHANDLER_H
#define SERIALCOMMANDHANDLER_H

#include <QString>
#include <QObject>
#include <QSerialPort>
#include <QVariant>

class SerialCommandHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString outgoingData READ getOutgoingData WRITE setOutgoingData NOTIFY outgoingDataReady)
    Q_PROPERTY(QString incomingData READ getIncomingData WRITE setIncomingData NOTIFY incomingDataChanged)
private:
    QString outgoingData_;
    QString incomingData_;
    QSerialPort& serial;

public:
    SerialCommandHandler(QSerialPort& serial, QObject* parent = nullptr);
    ~SerialCommandHandler();

    QString getOutgoingData() const;
    QString getIncomingData();

    void setOutgoingData(const QVariant& data);
    void setIncomingData(const QString& data);

    void sendSerialCommand(const QVariant& data);

signals:
    void outgoingDataReady(const QString& data);
    void outgoingDataWritten(const QByteArray& data);

    void incomingDataChanged();

};

#endif // SERIALCOMMANDHANDLER_H
