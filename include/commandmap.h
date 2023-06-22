#ifndef COMMANDMAP_H
#define COMMANDMAP_H

#include "commandhandler.h"

// CommandMap.h
#include <QString>
#include <QMap>
#include <functional>

class CommandMap
{
public:
    CommandMap() = default;
    ~CommandMap() = default;

    template <typename CommandType>
    void registerCommand(const QString& commandName, const std::function<void(QString)>& commandHandler)
    {
        commandHandlers_[commandName] = [commandHandler](QVariant arg) {
            commandHandler(arg.toString());
        };
    }

    void unregisterCommand(const QString& commandName)
    {
        commandHandlers_.remove(commandName);
    }

    bool hasCommand(const QString& commandName) const
    {
        return commandHandlers_.contains(commandName);
    }

    void executeCommand(const QString& commandName, QVariant argument)
    {
        auto it = commandHandlers_.find(commandName);
        if (it != commandHandlers_.end())
        {
            it.value()(argument);
        }
        else
        {
            // Handle the case when the command is not found
        }
    }

private:
    QMap<QString, std::function<void(QVariant)>> commandHandlers_;
};

#endif // COMMANDMAP_H
