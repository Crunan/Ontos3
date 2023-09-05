#ifndef LED_H
#define LED_H

#include <QString>
#include <QObject>

class LED : public QObject {

    Q_OBJECT

public:
    enum LEDType {
        Normal,
        Abort,
        EStop,
        PlasmaOn
    };
    Q_ENUM(LEDType)

    explicit LED(const QString& name, int index = -1, LEDType type = Normal, bool state = false);
    LED(const LED& other); // Copy constructor

    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(int index READ getIndex WRITE setIndex)
    Q_PROPERTY(LEDType type READ getType WRITE setType)
    Q_PROPERTY(bool state READ getState WRITE setState)

    QString getName() const;
    void setName(const QString& value);

    int getIndex() const;
    void setIndex(int index);

    LEDType getType() const;
    void setType(LEDType type);

    bool getState() const;
    void setState(bool state);

    void turnOn();
    void turnOff();
    bool isOn() const;

public:
    QString name;
    int index;
    LEDType type;
    bool state;
    static int nextIndex;
};

#endif // LED_H
