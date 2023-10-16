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
    m_heater(false),
    m_cycles(0),
    m_speed(0),
    m_overlap(0),
    m_gap(0),
    m_thickness(0),
    m_xMinPH(0),
    m_xMaxPH(0),
    m_yMinPH(0),
    m_yMaxPH(0)
{
}


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
    setPurgeFromRecipe();
    setHeaterFromRecipe();
    setCyclesFromRecipe();
}

//////////////////////////////////////////////////////////////////////////////////
// set from file
//////////////////////////////////////////////////////////////////////////////////
void PlasmaRecipe::setThicknessFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_THICKNESS_KEY)) {
        m_thickness = m_recipeMap[RECIPE_THICKNESS_KEY].toDouble();
        emit thicknessChanged();
    }
    else {
        // Handle the case when "THICKNESS" key is not found in the recipe map
        Logger::logWarning(RECIPE_THICKNESS_KEY + " setpoint not found in recipe map.");
    }
}
void PlasmaRecipe::setGapFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_GAP_KEY)) {
        m_gap = m_recipeMap[RECIPE_GAP_KEY].toDouble();
        emit gapChanged();
    }
    else {
        // Handle the case when "GAP" key is not found in the recipe map
        Logger::logWarning(RECIPE_GAP_KEY + " setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setOverlapFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_OVERLAP_KEY)) {
        m_overlap = m_recipeMap[RECIPE_OVERLAP_KEY].toDouble();
        emit overlapChanged();
    }
    else {
        // Handle the case when "OVERLAP" key is not found in the recipe map
        Logger::logWarning(RECIPE_OVERLAP_KEY + " setpoint not found in recipe map.");
    }

}

void PlasmaRecipe::setSpeedFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_SPEED_KEY)) {
        m_speed = m_recipeMap[RECIPE_SPEED_KEY].toDouble();
        emit speedChanged();
    }
    else {
        // Handle the case when "SPEED" key is not found in the recipe map
        Logger::logWarning(RECIPE_SPEED_KEY + " setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setAutoScanFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_AUTOSCAN_KEY)) {
        m_autoScan = m_recipeMap[RECIPE_AUTOSCAN_KEY].toBool();
        emit autoScanChanged();
    }
    else {
        // Handle the case when "AUTOSCAN" key is not found in the recipe map
        Logger::logWarning(RECIPE_AUTOSCAN_KEY + " setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setXaxisLimitsFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_XMIN_KEY)) {
        m_xMinPH = m_recipeMap[RECIPE_XMIN_KEY].toDouble();
    }
    else {
        // Handle the case when "XMIN" key is not found in the recipe map
        Logger::logWarning(RECIPE_XMIN_KEY + " setpoint not found in recipe map.");
    }
    if (m_recipeMap.contains(RECIPE_XMAX_KEY)) {
        m_xMaxPH = m_recipeMap[RECIPE_XMAX_KEY].toDouble();
    }
    else {
        // Handle the case when "XMAX" key is not found in the recipe map
        Logger::logWarning(RECIPE_XMAX_KEY + " setpoint not found in recipe map.");
    }

    emit xLimitsChanged();

}
void PlasmaRecipe::setYaxisLimitsFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_YMIN_KEY)) {
        m_yMinPH = m_recipeMap[RECIPE_YMIN_KEY].toDouble();
    }
    else {
        // Handle the case when "YMIN" key is not found in the recipe map
        Logger::logWarning(RECIPE_YMIN_KEY + " setpoint not found in recipe map.");
    }
    if (m_recipeMap.contains(RECIPE_YMAX_KEY)) {
        m_yMaxPH = m_recipeMap[RECIPE_YMAX_KEY].toDouble();
    }
    else {
        // Handle the case when "THICKNESS" key is not found in the recipe map
        Logger::logWarning(RECIPE_YMAX_KEY + " setpoint not found in recipe map.");
    }

    emit yLimitsChanged();
}

void PlasmaRecipe::setPurgeFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_PURGE_KEY)) {
        m_N2PurgeRecipe = m_recipeMap[RECIPE_PURGE_KEY].toBool();
    }
    else {
        // Handle the case when "PURGE" key is not found in the recipe map
        Logger::logWarning(RECIPE_PURGE_KEY + " setpoint not found in recipe map.");
    }

}
void PlasmaRecipe::setHeaterFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_HEATER_KEY)) {
        m_heater = m_recipeMap[RECIPE_HEATER_KEY].toBool();
    }
    else {
        // Handle the case when "HEATER" key is not found in the recipe map
        Logger::logWarning(RECIPE_HEATER_KEY + " setpoint not found in recipe map.");
    }
}

void PlasmaRecipe::setCyclesFromRecipe()
{
    if (m_recipeMap.contains(RECIPE_CYCLES_KEY)) {
        m_cycles = m_recipeMap[RECIPE_CYCLES_KEY].toInt();
        emit cyclesChanged();
    }
    else {
        // Handle the case when "CYCLES" key is not found in the recipe map
        Logger::logWarning(RECIPE_CYCLES_KEY + " setpoint not found in recipe map.");
    }
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
    emit autoScanChanged();
}

void PlasmaRecipe::setPurge(bool toggle)
{
    m_N2PurgeRecipe = toggle;
}

void PlasmaRecipe::setHeater(bool heater)
{
    m_heater = heater;
}

void PlasmaRecipe::setCycles(int cycles)
{
    m_cycles = cycles;
    emit cyclesChanged();
}

void PlasmaRecipe::setSpeed(double speed)
{
    m_speed = speed;
    emit speedChanged();
}

void PlasmaRecipe::setOverlap(double overlap)
{
    m_overlap = overlap;
    emit overlapChanged();
}

void PlasmaRecipe::setGap(double gap)
{
    m_gap = gap;
    emit gapChanged();
}

void PlasmaRecipe::setThickness(double thickness)
{
    m_thickness = thickness;
    emit thicknessChanged();
}

void PlasmaRecipe::setXminPH(double xmin)
{
    m_xMinPH = xmin;
    emit xLimitsChanged();
}

void PlasmaRecipe::setXmaxPH(double xmax)
{
    m_xMaxPH = xmax;
    emit xLimitsChanged();
}

void PlasmaRecipe::setYminPH(double ymin)
{
    m_yMinPH = ymin;
    emit yLimitsChanged();
}

void PlasmaRecipe::setYmaxPH(double ymax)
{
    m_yMaxPH = ymax;
    emit yLimitsChanged();
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


