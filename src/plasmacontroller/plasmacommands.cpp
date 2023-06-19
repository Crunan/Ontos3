#include "include/plasmacontroller/plasmacommands.h"


PlasmaCommands::PlasmaCommands()
{
    // Initialize the command map
    commandMap["BatchIDLogging"] = "$2A011%";
    commandMap["RecipeMBPosition"] = "$2A606%";
    commandMap["RecipeRFPosition"] = "$2A605%";
    commandMap["RecipeFlowMFC1"] = "$2A601";
    commandMap["RecipeFlowMFC2"] = "$2A602";
    commandMap["RecipeFlowMFC3"] = "$2A603";
    commandMap["RecipeFlowMFC4"] = "$2A604";
    commandMap["MFC1Range"] = "$8501";
    commandMap["MFC2Range"] = "$8502";
    commandMap["MFC3Range"] = "$8503";
    commandMap["MFC4Range"] = "$8504";
    commandMap["MaxRFPowerForward"] = "$2A705%";
    commandMap["AutoMan"] = "$89%";
    commandMap["Temp"] = "$8C%";
    commandMap["CTLStatus"] = "$91%";
    commandMap["ResetCTL"] = "$90%";
    commandMap["SetExecRecipeOn"] = "$8701";
    commandMap["SetExecRecipeOff"] = "$8700";
    commandMap["HowManyMFCs"] = "$2A002%";
    commandMap["TuneDown"] = "$110000";
    commandMap["TuneUp"] = "$110100";
    commandMap["SetBatchIDOn"] = "$28011;1";
    commandMap["SetBatchIDOff"] = "$28011;0";
}

QString PlasmaCommands::getCommandString(const QString& commandName) const
{
    // Search for the command in the map
    auto it = commandMap.find(commandName);
    if (it != commandMap.end())
    {
        // Command found, return the associated string
        return it.value();
    }
    else
    {
        // Command not found, return an empty string or handle the error as needed
        return "";
    }
}

QStringList PlasmaCommands::getAllCommands()
{
    return commandMap.keys();
}
