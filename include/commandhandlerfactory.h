#ifndef COMMANDHANDLERFACTORY_H
#define COMMANDHANDLERFACTORY_H

#include "commandfilereader.h"
#include "commandhandler.h"

#include <memory>
#include <unordered_map>
#include <functional>

class CommandHandlerFactory
{
public:
    template <typename CommandType>
    void registerCommandHandler(const std::string& commandName)
    {
        commandHandlerRegistry_[commandName] = []() {
            return std::make_unique<CommandHandler<CommandType>>(CommandType{});
        };
    }

    std::unique_ptr<CommandHandlerBase> createCommandHandler(const std::string& commandName)
    {
        if (commandHandlerRegistry_.count(commandName) > 0)
        {
            return commandHandlerRegistry_[commandName]();
        }

        // Handle the case when the command name is not registered
        // Return a default command handler or throw an exception
        // based on your specific requirements

        return nullptr;
    }

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<CommandHandlerBase>()>> commandHandlerRegistry_;
};

#endif // COMMANDHANDLERFACTORY_H
