#include "plasmarecipe.h"

#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>
#include "logger.h"
#include "UtilitiesAndConstants.h"


PlasmaRecipe::PlasmaRecipe(QObject* parent) :
    QObject(parent),
    m_recipeMap(),
    m_cascadeRecipeList(),
    m_currentRecipeIndex(0)
{
    initializeMap();
}


PlasmaRecipe::~PlasmaRecipe() {
    // Cleanup any resources here
}

void PlasmaRecipe::initializeMap()
{
    m_recipeMap.insert(RECIPE_MFC1_KEY, 0);
    m_recipeMap.insert(RECIPE_MFC2_KEY, 0);
    m_recipeMap.insert(RECIPE_MFC3_KEY, 0);
    m_recipeMap.insert(RECIPE_MFC4_KEY, 0);
    m_recipeMap.insert(RECIPE_MFC5_KEY, 0);
    m_recipeMap.insert(RECIPE_MFC6_KEY, 0);
    m_recipeMap.insert(RECIPE_PWR_KEY, 0);
    m_recipeMap.insert(RECIPE_TUNER_KEY, 0);
    m_recipeMap.insert(RECIPE_THICKNESS_KEY, 1.0);
    m_recipeMap.insert(RECIPE_GAP_KEY, 1.0);
    m_recipeMap.insert(RECIPE_OVERLAP_KEY, 0);
    m_recipeMap.insert(RECIPE_SPEED_KEY, 1.0);
    m_recipeMap.insert(RECIPE_CYCLES_KEY, 1);
    m_recipeMap.insert(RECIPE_XMIN_KEY, 0);
    m_recipeMap.insert(RECIPE_XMAX_KEY, 0);
    m_recipeMap.insert(RECIPE_YMIN_KEY, 0);
    m_recipeMap.insert(RECIPE_YMAX_KEY, 0);
    m_recipeMap.insert(RECIPE_PURGE_KEY, false);
    m_recipeMap.insert(RECIPE_AUTOSCAN_KEY, false);
    m_recipeMap.insert(RECIPE_HEATER_KEY, false);
}

void PlasmaRecipe::setRecipeFromFile()
{
    QString absoluteFilePath = fileReader.getFilePath();

    QFile file(absoluteFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        QStringList list = line.split(">");

        if (list.length() == 2) { // found a recipe entry

            QString name = list.at(0); // get the key
            QString value = list.at(1); // get the value

            if (name.at(0) == '<') { // format check
                name = name.remove(0, 1); // remove the "<"
                if (name == RECIPE_THICKNESS_KEY) setThickness(value.toDouble());
                else if (name == RECIPE_GAP_KEY) setGap(value.toDouble());
                else if (name == RECIPE_OVERLAP_KEY) setOverlap(value.toDouble());
                else if (name == RECIPE_SPEED_KEY) setSpeed(value.toDouble());
                else if (name == RECIPE_CYCLES_KEY) setCycles(value.toInt());
                else if (name == RECIPE_XMIN_KEY) setXminPH(value.toDouble());
                else if (name == RECIPE_XMAX_KEY) setXmaxPH(value.toDouble());
                else if (name == RECIPE_YMIN_KEY) setYminPH(value.toDouble());
                else if (name == RECIPE_YMAX_KEY) setYmaxPH(value.toDouble());
                else if (name == RECIPE_PURGE_KEY) setPurge(QVariant(value).toBool());
                else if (name == RECIPE_AUTOSCAN_KEY) setAutoScan(QVariant(value).toBool());
                else if (name == RECIPE_HEATER_KEY) setHeater(QVariant(value).toBool());
                else m_recipeMap[name] = value;
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

void PlasmaRecipe::setAutoScan(bool toggle)
{
    m_recipeMap[RECIPE_AUTOSCAN_KEY] = toggle;
    emit autoScanChanged();
}

bool PlasmaRecipe::getAutoScanBool() const
{
    return m_recipeMap[RECIPE_AUTOSCAN_KEY].toBool();
}
QString PlasmaRecipe::getAutoScanQStr() const
{
    return QString::number(m_recipeMap[RECIPE_AUTOSCAN_KEY].toBool());
}

void PlasmaRecipe::setPurge(bool toggle)
{
    m_recipeMap[RECIPE_PURGE_KEY] = toggle;
}

bool PlasmaRecipe::getPurge() const
{
    return m_recipeMap[RECIPE_PURGE_KEY].toBool();
}
QString PlasmaRecipe::getPurgeQStr() const
{
    return QString::number(m_recipeMap[RECIPE_PURGE_KEY].toBool());
}

void PlasmaRecipe::setHeater(bool heater)
{
    m_recipeMap[RECIPE_HEATER_KEY] = heater;
}

bool PlasmaRecipe::getHeater() const
{
    return m_recipeMap[RECIPE_HEATER_KEY].toBool();
}
QString PlasmaRecipe::getHeaterQStr() const
{
    return QString::number(m_recipeMap[RECIPE_HEATER_KEY].toBool());
}

void PlasmaRecipe::setCycles(int cycles)
{
    m_recipeMap[RECIPE_CYCLES_KEY] = cycles;
    emit cyclesChanged();
}

QString PlasmaRecipe::getCyclesQStr() const
{
    return QString::number(m_recipeMap[RECIPE_CYCLES_KEY].toInt());
}
int PlasmaRecipe::getCyclesInt() const
{
    return m_recipeMap[RECIPE_CYCLES_KEY].toInt();
}

void PlasmaRecipe::setSpeed(double speed)
{
    m_recipeMap[RECIPE_SPEED_KEY] = speed;
    emit speedChanged();
}

double PlasmaRecipe::getSpeed() const
{
    return m_recipeMap[RECIPE_SPEED_KEY].toDouble();
}
QString PlasmaRecipe::getSpeedQStr() const
{
    return QString::number(m_recipeMap[RECIPE_SPEED_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setOverlap(double overlap)
{
    m_recipeMap[RECIPE_OVERLAP_KEY] = overlap;
    emit overlapChanged();
}

double PlasmaRecipe::getOverlap() const
{
    return m_recipeMap[RECIPE_OVERLAP_KEY].toDouble();
}
QString PlasmaRecipe::getOverlapQStr() const
{
    return QString::number(m_recipeMap[RECIPE_OVERLAP_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setGap(double gap)
{
    m_recipeMap[RECIPE_GAP_KEY] = gap;
    emit gapChanged();
}

double PlasmaRecipe::getGap() const
{
    return m_recipeMap[RECIPE_GAP_KEY].toDouble();
}
QString PlasmaRecipe::getGapQStr() const
{
    return QString::number(m_recipeMap[RECIPE_GAP_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setThickness(double thickness)
{
    m_recipeMap[RECIPE_THICKNESS_KEY] = thickness;
    emit thicknessChanged();
}

double PlasmaRecipe::getThickness() const
{
    return m_recipeMap[RECIPE_THICKNESS_KEY].toDouble();
}

QString PlasmaRecipe::getThicknessQStr() const
{
    return QString::number(m_recipeMap[RECIPE_THICKNESS_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setXminPH(double xmin)
{
    m_recipeMap[RECIPE_XMIN_KEY] = xmin;
    emit xLimitsChanged();
}

double PlasmaRecipe::getXminPH() const
{
    return m_recipeMap[RECIPE_XMIN_KEY].toDouble();
}
QString PlasmaRecipe::getXminPHQStr() const
{
    return QString::number(m_recipeMap[RECIPE_XMIN_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setXmaxPH(double xmax)
{
    m_recipeMap[RECIPE_XMAX_KEY] = xmax;
    emit xLimitsChanged();
}

double PlasmaRecipe::getXmaxPH() const
{
    return m_recipeMap[RECIPE_XMAX_KEY].toDouble();
}
QString PlasmaRecipe::getXmaxPHQStr() const
{
    return QString::number(m_recipeMap[RECIPE_XMAX_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setYminPH(double ymin)
{
    m_recipeMap[RECIPE_YMIN_KEY] = ymin;
    emit yLimitsChanged();
}

double PlasmaRecipe::getYminPH() const
{
    return m_recipeMap[RECIPE_YMIN_KEY].toDouble();
}
QString PlasmaRecipe::getYminPHQStr() const
{
    return QString::number(m_recipeMap[RECIPE_YMIN_KEY].toDouble(), 'f', 2);
}

void PlasmaRecipe::setYmaxPH(double ymax)
{
    m_recipeMap[RECIPE_YMAX_KEY] = ymax;
    emit yLimitsChanged();
}

double PlasmaRecipe::getYmaxPH() const
{
    return m_recipeMap[RECIPE_YMAX_KEY].toDouble();
}

QString PlasmaRecipe::getYmaxPHQStr() const
{
    return QString::number(m_recipeMap[RECIPE_YMAX_KEY].toDouble(), 'f', 2);
}

QMap<QString, QVariant> PlasmaRecipe::getRecipeMap()
{
    return m_recipeMap;
}

QList<QString> PlasmaRecipe::getCascadeRecipeList()
{
    return m_cascadeRecipeList;
}
void PlasmaRecipe::addRecipeToCascade(const QString& recipeName)
{
    m_cascadeRecipeList.append(recipeName);
}

void PlasmaRecipe::removeRecipeFromCascade(const QString& recipeName)
{
    m_cascadeRecipeList.removeOne(recipeName);
}

void PlasmaRecipe::clearCascadeRecipes()
{
    m_cascadeRecipeList.clear();
    m_currentRecipeIndex = 0;
}




