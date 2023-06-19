#ifndef AXISCONTROLLER_H
#define AXISCONTROLLER_H

#include <QObject>

//class AxisController : public QObject {
//    Q_OBJECT
//    Q_PROPERTY(QString currentStatus READ getCurrentStatus WRITE setCurrentStatus NOTIFY currentStatusChanged)
//    Q_PROPERTY(QStringList axisStatus READ getAxisStatus NOTIFY axisStatusChanged)
//    Q_PROPERTY(int ledStates READ getLedStates NOTIFY ledStatesChanged)
//    Q_PROPERTY(bool doorsOpen READ areDoorsOpen NOTIFY doorsOpenChanged)
//    Q_PROPERTY(bool joystickOn READ isJoystickOn WRITE setJoystickOn NOTIFY joystickOnChanged)
//    Q_PROPERTY(bool vacuumOn READ isVacuumOn WRITE setVacuumOn NOTIFY vacuumOnChanged)
//    Q_PROPERTY(bool nitrogenPurgeOn READ isNitrogenPurgeOn WRITE setNitrogenPurgeOn NOTIFY nitrogenPurgeOnChanged)

//private:
//    QString currentStatus;
//    QStringList axisStatus;
//    int ledStates;
//    bool doorsOpen;
//    bool joystickOn;
//    bool vacuumOn;
//    bool nitrogenPurgeOn;

//public:
//    Controller(QObject* parent = nullptr);
//    ~Controller();

//    QString getCurrentStatus() const;
//    void setCurrentStatus(const QString& status);

//    QStringList getAxisStatus() const;
//    void setAxisStatus(const QStringList& statusList);

//    int getLedStates() const;
//    void setLedStates(int states);

//    bool areDoorsOpen() const;
//    void setDoorsOpen(bool isOpen);

//    bool isJoystickOn() const;
//    void setJoystickOn(bool isOn);

//    bool isVacuumOn() const;
//    void setVacuumOn(bool isOn);

//    bool isNitrogenPurgeOn() const;
//    void setNitrogenPurgeOn(bool isOn);

//    bool sameStateXYZ();
//signals:
//    void currentStatusChanged();
//    void axisStatusChanged();
//    void ledStatesChanged();
//    void doorsOpenChanged();
//    void joystickOnChanged();
//    void vacuumOnChanged();
//    void nitrogenPurgeOnChanged();
//};

#endif // AXISCONTROLLER_H
