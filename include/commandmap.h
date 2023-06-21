#ifndef COMMANDMAP_H
#define COMMANDMAP_H

#include "commandhandler.h"

#include <QString>
#include <QMap>
#include <functional>
#include <memory>

class CommandMap
{
public:
    CommandMap() = default;
    ~CommandMap() = default;

    template <typename CommandType>
    void registerCommand(const QString& commandName, const CommandType& command)
    {
        commandHandlers_[commandName] = std::make_shared<CommandHandler<CommandType>>(command);
    }

    void unregisterCommand(const QString& commandName)
    {
        commandHandlers_.remove(commandName);
    }

    bool hasCommand(const QString& commandName) const
    {
        return commandHandlers_.contains(commandName);
    }

    void executeCommand(const QString& commandName)
    {
        auto it = commandHandlers_.find(commandName);
        if (it != commandHandlers_.end())
        {
            it.value()->handleCommand();
        }
        else
        {
            // Handle the case when the command is not found
        }
    }

private:
    QMap<QString, std::shared_ptr<CommandHandlerBase>> commandHandlers_;
};

#endif // COMMANDMAP_H
