#ifndef PLASMACOMMANDS_H
#define PLASMACOMMANDS_H

#include <QMap>
#include <QString>

class PlasmaCommands
{
public:
    PlasmaCommands();

    QString getCommandString(const QString& commandName) const;
    QStringList getAllCommands();

private:
    QMap<QString, QString> commandMap;
};

#endif // PLASMACOMMANDS_H
