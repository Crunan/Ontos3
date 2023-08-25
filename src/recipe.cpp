#include "include/recipe.h"

Recipe::Recipe(QObject* parent)
    : QObject(parent),
    //m_plasmaRecipe(),
    //m_scanRecipe(),
    m_autoScanFlag(false),
    m_autoScan(false),
    m_N2PurgeRecipe(false),
    m_cycles(0),
    m_speed(0),
    m_overlap(0),
    m_gap(0),
    m_thickness(0)
{}

Recipe::~Recipe()
{}






