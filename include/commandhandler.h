#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <functional>


class CommandHandlerBase
/* This is an abstract base class that defines the interface
 * for command handlers. It should have methods for sending
 * the command to the controller and processing the
 * incoming data.
 */
{
public:
    virtual ~CommandHandlerBase() {}
    virtual void handleCommand() = 0;
};

template <typename CommandType>
class CommandHandler : public CommandHandlerBase
{
public:
    explicit CommandHandler(CommandType&& command) : command_(std::move(command)) {}

//    explicit CommandHandler(const CommandType&& command) : command_(command) {}

    void handleCommand() override
    {
        // Implement the command handling logic here
        // using the stored command
    }

private:
    CommandType command_;
};

#endif // COMMANDHANDLER
