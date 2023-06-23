#include "commandmap.h"

CommandMap::CommandMap()
{
}

void CommandMap::setCommandMap(const QMap<QString, QPair<QString, QString>>& map)
{
    commandMap_ = map;
}

const QMap<QString, QPair<QString, QString>>& CommandMap::getCommandMap() const
{
    return commandMap_;
}

const QString CommandMap::findCommand(const QString& command) const
{
    if (commandMap_.contains(command)) {
        const QPair<QString, QString>& pair = commandMap_.value(command);
        return pair.first;  // Retrieve the first value
    }
    else {
        return "";  // Command not found, return an appropriate value or handle the case accordingly
    }
}
