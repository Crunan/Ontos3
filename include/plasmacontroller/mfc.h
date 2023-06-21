#ifndef MFC_H
#define MFC_H

#include <QObject>

#include "commandhandler.h"
#include "commandmap.h"

class MFC : public QObject {

    Q_OBJECT
    Q_PROPERTY(double loadedSetpoint READ getSetpoint WRITE setSetpoint NOTIFY setpointChanged)
    Q_PROPERTY(double actualFlow READ getFlow NOTIFY flowChanged)
    Q_PROPERTY(double range READ getRange CONSTANT)

    double loadedSetpoint;
    double actualFlow;
    double range;

public:
    MFC();

    double getSetpoint() const;
    void setSetpoint(double value);

    double getFlow() const;
    double getRange() const;

signals:
    void setpointChanged();
    void flowChanged();

private:
    CommandHandler<double> setpointCommandHandler;
    CommandHandler<double> flowCommandHandler;
    CommandHandler<double> rangeCommandHandler;

};


#endif // MFC_H
