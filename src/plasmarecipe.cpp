#include "plasmarecipe.h"

#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>
#include "logger.h"


PlasmaRecipe::PlasmaRecipe(QObject* parent) :
    QObject(parent),
    m_recipeMap(),
    m_cascadeRecipeList(),
    m_currentRecipeIndex(0),
    m_autoScanFlag(false),
    m_autoScan(false),
    m_N2PurgeRecipe(false),
    m_cycles(0),
    m_speed(0),
    m_overlap(0),
    m_gap(0),
    m_thickness(0),
    m_xMin(0),
    m_xMax(0),
    m_yMin(0),
    m_yMax(0)
{}



PlasmaRecipe::~PlasmaRecipe() {
    // Cleanup any resources here
}

void PlasmaRecipe::setRecipeFromFile()
{
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
    }

    file.close();
}

void PlasmaRecipe::processRecipeKeys()
{
    setThicknessFromRecipe();
    setGapFromRecipe();
    setOverlapFromRecipe();
    setSpeedFromRecipe();
    setAutoScanFromRecipe();
    setXaxisLimitsFromRecipe();
    setYaxisLimitsFromRecipe();
}

//////////////////////////////////////////////////////////////////////////////////
// set from file
//////////////////////////////////////////////////////////////////////////////////
void PlasmaRecipe::setThicknessFromRecipe()
{
    if (m_recipeMap.contains("THICKNESS")) {
        m_thickness = m_recipeMap["THICKNESS"].toDouble();
        emit thicknessChanged(m_thickness);
    }
    else {
        // Handle the case when "THICKNESS" key is not found in the recipe map
        Logger::logWarning("THICKNESS setpoint not found in recipe map.");
    }
}
void PlasmaRecipe::setGapFromRecipe()
{
    if (m_recipeMap.contains("GAP")) {
        m_gap = m_recipeMap["GAP"].toDouble();
        emit gapChanged(m_gap);
    }
    else {
        // Handle the case when "GAP" key is not found in the recipe map
        Logger::logWarning("GAP setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setOverlapFromRecipe()
{
    if (m_recipeMap.contains("OVERLAP")) {
        m_overlap = m_recipeMap["OVERLAP"].toDouble();
        emit overlapChanged(m_overlap);
    }
    else {
        // Handle the case when "OVERLAP" key is not found in the recipe map
        Logger::logWarning("OVERLAP setpoint not found in recipe map.");
    }

}

void PlasmaRecipe::setSpeedFromRecipe()
{
    if (m_recipeMap.contains("SPEED")) {
        m_speed = m_recipeMap["SPEED"].toDouble();
        emit speedChanged(m_speed);
    }
    else {
        // Handle the case when "SPEED" key is not found in the recipe map
        Logger::logWarning("SPEED setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setAutoScanFromRecipe()
{
    if (m_recipeMap.contains("AUTOSCAN")) {
        m_autoScan = m_recipeMap["AUTOSCAN"].toDouble();
        emit autoScanChanged(m_autoScan);
    }
    else {
        // Handle the case when "AUTOSCAN" key is not found in the recipe map
        Logger::logWarning("AUTOSCAN setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setXaxisLimitsFromRecipe()
{
    if (m_recipeMap.contains("XMIN")) {
        m_xMin = m_recipeMap["XMIN"].toDouble();
    }
    else {
        // Handle the case when "XMIN" key is not found in the recipe map
        Logger::logWarning("XMIN setpoint not found in recipe map.");
    }
    if (m_recipeMap.contains("XMAX")) {
        m_xMax = m_recipeMap["XMAX"].toDouble();
    }
    else {
        // Handle the case when "XMAX" key is not found in the recipe map
        Logger::logWarning("XMAX setpoint not found in recipe map.");
    }

    emit xLimitsChanged(m_xMin, m_xMax);

}
void PlasmaRecipe::setYaxisLimitsFromRecipe()
{
    if (m_recipeMap.contains("YMIN")) {
        m_yMin = m_recipeMap["YMIN"].toDouble();
    }
    else {
        // Handle the case when "YMIN" key is not found in the recipe map
        Logger::logWarning("YMIN setpoint not found in recipe map.");
    }
    if (m_recipeMap.contains("YMAX")) {
        m_yMax = m_recipeMap["YMAX"].toDouble();
    }
    else {
        // Handle the case when "THICKNESS" key is not found in the recipe map
        Logger::logWarning("YMAX setpoint not found in recipe map.");
    }

    emit yLimitsChanged(m_yMin, m_yMax);
}

//////////////////////////////////////////////////////////////////////////////////
// set externally
//////////////////////////////////////////////////////////////////////////////////
void PlasmaRecipe::setAutoScanFlag(bool toggle)
{
    m_autoScanFlag = toggle;
}


void PlasmaRecipe::setAutoScan(bool toggle)
{
    m_autoScan = toggle;
    emit autoScanChanged(m_autoScan);
}

void PlasmaRecipe::setPurge(bool toggle)
{
    m_N2PurgeRecipe = toggle;
}

void PlasmaRecipe::setCycles(int cycles)
{
    m_cycles = cycles;
}

void PlasmaRecipe::setSpeed(double speed)
{
    m_speed = speed;
    emit speedChanged(m_speed);
}

void PlasmaRecipe::setOverlap(double overlap)
{
    m_overlap = overlap;
    emit overlapChanged(m_overlap);
}

void PlasmaRecipe::setGap(double gap)
{
    m_gap = gap;
    emit gapChanged(m_gap);
}

void PlasmaRecipe::setThickness(double thickness)
{
    m_thickness = thickness;
    emit thicknessChanged(m_gap);
}

void PlasmaRecipe::setXmin(double xmin)
{
    m_xMin = xmin;
    emit xLimitsChanged(m_yMin, m_yMax);
}

void PlasmaRecipe::setXmax(double xmax)
{
    m_xMax = xmax;
    emit xLimitsChanged(m_yMin, m_yMax);
}

void PlasmaRecipe::setYmin(double ymin)
{
    m_yMin = ymin;
    emit yLimitsChanged(m_yMin, m_yMax);
}

void PlasmaRecipe::setYmax(double ymax)
{
    m_yMax = ymax;
    emit yLimitsChanged(m_yMin, m_yMax);
}


QMap<QString, QVariant> PlasmaRecipe::getRecipeMap()
{
    return m_recipeMap;
}

QList<QString> PlasmaRecipe::getCascadeRecipeList()
{
    return m_cascadeRecipeList;
}
void PlasmaRecipe::addRecipeToCascade(const QString& recipeName) {
    m_cascadeRecipeList.append(recipeName);
}

void PlasmaRecipe::removeRecipeFromCascade(const QString& recipeName) {
    m_cascadeRecipeList.removeOne(recipeName);
}

void PlasmaRecipe::executeCurrentRecipe() {
    if (m_currentRecipeIndex >= 0 && m_currentRecipeIndex < m_cascadeRecipeList.size()) {
        const QString& recipeName = m_cascadeRecipeList.at(m_currentRecipeIndex);
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


