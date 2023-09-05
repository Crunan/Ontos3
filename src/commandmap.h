#ifndef COMMANDMAP_H
#define COMMANDMAP_H

#include <QMap>
#include <QPair>
#include <QString>

class CommandMap
{
public:
    CommandMap();  // Default constructor

    const QMap<QString, QPair<QString, QString>>& getCommandMap() const;
    void setCommandMap(const QMap<QString, QPair<QString, QString>>& map);

    const QString findCommandValue(const QString& command) const;

private:
    QMap<QString, QPair<QString, QString>> commandMap_;
};

#endif // COMMANDMAP_H
