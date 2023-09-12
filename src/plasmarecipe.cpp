#include "plasmarecipe.h"

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
    // this must refer to a different recipe format: static QRegularExpression regex("([^=]+)=(.*)");

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

        QStringList list = line.split(">");

        if (list.length() == 2) { // found a recipe entry

            QString name = list.at(0); // get the key
            QString setpoint = list.at(1); // get the setpoint

            if (name.at(0) == '<') { // format check
                name = name.remove(0, 1); // remove the "<"
                m_recipeMap[name] = setpoint;
            }
            else {
                qDebug() << "Invalid line format:" << line;
            }

        }
        else {
            qDebug() << "Invalid line format:" << line;
        }

        /*
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
        }*/
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
    for (int i = 0; i < CTL_->m_mfcs.size(); i++)
    {
        MFC* mfc = CTL_->m_mfcs.at(i);
        QString mfcKey = "MFC" + QString::number(i + 1);

        if (m_recipeMap.contains(mfcKey)) {
            double flow = m_recipeMap[mfcKey].toDouble();
            mfc->setRecipeFlow(flow);
        }
        else  {
            // Handle the case when the MFC key is not found in the recipe map
            qDebug() << "MFC" << i+1 << "setpoint not found in recipe map.";
        }
    }
}

void PlasmaRecipe::setRFSetpoint() {
    if (m_recipeMap.contains("RF")) {
        int watts = m_recipeMap["RF"].toInt();
        CTL_->m_pwr.setRecipeWatts(watts);
    }
    else {
        // Handle the case when "RF" key is not found in the recipe map
        qDebug() << "RF setpoint not found in recipe map.";
    }
}

void PlasmaRecipe::setTunerSetpoint() {
    if (m_recipeMap.contains("TUNER")) {
        double position = m_recipeMap["TUNER"].toDouble();
        CTL_->m_tuner.setRecipePosition(position);
    }
    else {
        // Handle the case when "TUNER" key is not found in the recipe map
        qDebug() << "TUNER setpoint not found in recipe map.";
    }
}

void PlasmaRecipe::setAutoTuneOn() {
    if (m_recipeMap.contains("AUTO")) {
        QVariant value = m_recipeMap["AUTO"];

        if (value.canConvert<bool>()) {
            bool booleanValue = value.toBool();
            CTL_->m_tuner.setAutoTune(booleanValue);
        }
        else {
            qDebug() << "auto tune value is not a boolean.";
        }
    }
    else {
        qDebug() << "auto tune setpoint not found in recipe map.";
    }
}

QMap<QString, QVariant> PlasmaRecipe::getRecipeMap()
{
    return m_recipeMap;
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
    if (m_currentRecipeIndex >= 0 && m_currentRecipeIndex < cascadeRecipeList_.size()) {
        const QString& recipeName = cascadeRecipeList_.at(m_currentRecipeIndex);
        // Load and execute the recipe with the given recipeName
        fileReader.setFilePath(recipeName);
        setRecipeFromFile();
        // Once the recipe execution is complete, move to the next recipe
        ++m_currentRecipeIndex;
        executeCurrentRecipe();
    } else {
        // All recipes in the cascade have been executed
        // Perform any final actions or cleanup
        m_currentRecipeIndex = 0;
    }
}

bool PlasmaRecipe::isRecipeComplete()
{
    //if (Stage.scan.isFinished())
        //return true;
}


