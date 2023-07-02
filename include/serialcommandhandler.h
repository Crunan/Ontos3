#ifndef SERIALCOMMANDHANDLER_H
#define SERIALCOMMANDHANDLER_H

#include <QString>
#include <QObject>
#include <QSerialPort>
#include <functional>
#include <QVariant>

class SerialCommandHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString outgoingData_ READ getOutgoingData  WRITE setOutgoingData NOTIFY outgoingDataChanged)
    Q_PROPERTY(QString incomingData_ READ getIncomingData NOTIFY incomingDataChanged)
private:
    QString outgoingData_;
    QString incomingData_;

public:
    SerialCommandHandler(QSerialPort& serial, QObject *parent = nullptr);
    ~SerialCommandHandler();


    QString getOutgoingData() const;
    void setOutgoingData(const QVariant& data);

    QString getIncomingData();
    void setIncomingData();

    void send(QString& data);

signals:
    void outgoingDataChanged();
    void incomingDataChanged();

public slots:
    void writeOutgoingData();

public:
    QSerialPort& serial;
};

#endif // SERIALCOMMANDHANDLER_H
