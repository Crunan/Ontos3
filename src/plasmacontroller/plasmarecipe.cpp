#include "include/plasmacontroller/plasmarecipe.h"

#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>


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

    QString absoluteFilePath = fileReader.getFilePath();

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
    processRecipeKeys();
    file.close();
}

void PlasmaRecipe::processRecipeKeys()
{
    setMFCsActualFlow();
    setRFSetpoint();
    setTunerSetpoint();
    setAutoTuneOn();
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
            mfc->setRecipeFlow(flow);
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
    if (recipeMap_.contains("AUTO")) {
        QVariant value = recipeMap_["AUTO"];

        if (value.canConvert<bool>()) {
            bool booleanValue = value.toBool();
            CTL_->tuner.setAutoTune(booleanValue);
        } else {
            qDebug() << "auto tune value is not a boolean.";
        }
    } else {
        qDebug() << "auto tune setpoint not found in recipe map.";
    }
}

QMap<QString, QVariant> PlasmaRecipe::getRecipeMap()
{
    return recipeMap_;
}

QList<QString> PlasmaRecipe::getCascadeRecipeList()
{
    return cascadeRecipeList_;
}
void PlasmaRecipe::addRecipeToCascade(const QString& recipeName) {
    cascadeRecipeList_.append(recipeName);
}

void PlasmaRecipe::removeRecipeFromCascade(const QString& recipeName) {
    cascadeRecipeList_.removeOne(recipeName);
}

void PlasmaRecipe::executeCurrentRecipe() {
    if (currentRecipeIndex_ >= 0 && currentRecipeIndex_ < cascadeRecipeList_.size()) {
        const QString& recipeName = cascadeRecipeList_.at(currentRecipeIndex_);
        // Load and execute the recipe with the given recipeName
        fileReader.setFilePath(recipeName);
        setRecipeFromFile();
        // Once the recipe execution is complete, move to the next recipe
        ++currentRecipeIndex_;
        executeCurrentRecipe();
    } else {
        // All recipes in the cascade have been executed
        // Perform any final actions or cleanup
        currentRecipeIndex_ = 0;
    }
}

bool PlasmaRecipe::isRecipeComplete()
{
    //if (Stage.scan.isFinished())
        //return true;
}


