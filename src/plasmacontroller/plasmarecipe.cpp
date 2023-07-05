#include "include/plasmacontroller/plasmarecipe.h"



PlasmaRecipe::PlasmaRecipe(PlasmaController* CTL, QObject* parent)
    : QObject(parent), CTL_(CTL)
{
}

PlasmaRecipe::~PlasmaRecipe() {
    // Cleanup any resources here
}

void PlasmaRecipe::setRecipeFromFile()
{
    static QRegularExpression regex("([^=]+)=(.*)");

    QString absoluteFilePath = QCoreApplication::applicationDirPath() + "/" + fileReader_.getFilePath() + fileReader_.getFileName();

    QFile file(absoluteFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open the file:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch())
        {
            QString name = match.captured(1);
            QString setpoint = match.captured(2);

            if (!setpoint.isEmpty())
            {
                recipeMap_[name] = setpoint;
            }
            else
            {
                qDebug() << "Empty setpoint for line:" << line;
            }
        }
        else
        {
            qDebug() << "Invalid line format:" << line;
        }
    }

    file.close();
}

void PlasmaRecipe::setMFCsActualFlow()
{
    for (int i = 0; i < CTL_->mfcs.size(); i++)
    {
        MFC* mfc = CTL_->mfcs.at(i);
        QString mfcKey = "MFC" + QString::number(i + 1);

        if (recipeMap_.contains(mfcKey))
        {
            double flow = recipeMap_[mfcKey].toDouble();
            mfc->setActualFlow(flow);
        }
        else
        {
            // Handle the case when the MFC key is not found in the recipe map
            qDebug() << "MFC" << i+1 << "setpoint not found in recipe map.";
        }
    }
}

void PlasmaRecipe::setRFSetpoint() {
    if (recipeMap_.contains("RF"))
    {
        int watts = recipeMap_["RF"].toInt();
        CTL_->pwr.setRecipeWatts(watts);
    }
    else
    {
        // Handle the case when "RF" key is not found in the recipe map
        qDebug() << "RF setpoint not found in recipe map.";
    }
}

void PlasmaRecipe::setTunerSetpoint() {
    if (recipeMap_.contains("TUNER"))
    {
        double position = recipeMap_["TUNER"].toDouble();
        CTL_->tuner.setRecipePosition(position);
    }
    else
    {
        // Handle the case when "TUNER" key is not found in the recipe map
        qDebug() << "TUNER setpoint not found in recipe map.";
    }
}

void PlasmaRecipe::setAutoTuneOn() {
    if (recipeMap_.contains("AUTO"))
    {
        double setpoint = recipeMap_["AUTO"].toInt();
        CTL_->tuner.setAutoTune(setpoint);
    }
    else
    {
        // Handle the case when "AUTO Tune" key is not found in the recipe map
        qDebug() << "AUTO setpoint not found in recipe map.";
    }
}
