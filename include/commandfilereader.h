#ifndef COMMANDFILEREADER_H
#define COMMANDFILEREADER_H

#include "include/filereader.h"

#include <QString>
#include <QMap>
#include <QPair>

class CommandFileReader

{
public:
    CommandFileReader();

    QMap<QString, QPair<QString, QString>> readCommandsFromFile();


private:
    FileReader commandFile_;
};


#endif // COMMANDFILEREADER_H
