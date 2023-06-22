#ifndef COMMANDHANDLERFACTORY_H
#define COMMANDHANDLERFACTORY_H

#include <memory>
#include <unordered_map>
#include <functional>
#include "commandmap.h"

class CommandHandlerFactory
{
public:
    template <typename CommandType>
    void registerCommand(const QString& commandName, const std::function<void(CommandType)>& commandHandler)
    {
        commandMap_.registerCommand(commandName, commandHandler);
    }

    void unregisterCommand(const QString& commandName)
    {
        commandMap_.unregisterCommand(commandName);
    }

    CommandMap& getCommandMap()
    {
        return commandMap_;
    }

private:
    CommandMap commandMap_;
};


#endif // COMMANDHANDLERFACTORY_H
