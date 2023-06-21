#ifndef CONTROLLERINTERFACE_H
#define CONTROLLERINTERFACE_H


class ControllerInterface
/* This class acts as the interface between your application
 * and the controller. It uses the CommandMap to retrieve the
 * appropriate command handler for a given command
 * and delegates the command execution to the handler.
 * It should also handle the initialization of the
 * SerialComms object and provide methods for invoking
 * specific commands.
 */

{
public:
    ControllerInterface();
};

#endif // CONTROLLERINTERFACE_H
