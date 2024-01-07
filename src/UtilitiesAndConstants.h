#ifndef UTILITIESANDCONSTANTS_H
#define UTILITIESANDCONSTANTS_H

#include <QString>

bool isBitSet(int test_int, int bit_pos);


static const char LOG_DIRECTORY[] = "/opt/OTT_PLUS/logs/";
// settings keys
static const char COLLISION_SYSTEM_ENABLED_SETTING[] = "/toolSettings/collisionSystemEnabled";
static const char HEATER_ENABLED_SETTING[] = "/toolSettings/heaterEnabled";
static const char BATCHID_ENABLED_SETTING[] = "/toolSettings/batchIDEnabled";
static const char LED_LIGHT_INTENSITY[] = "/toolSettings/LEDLightIntensity";

// recipe key
static const char RECIPE_PWR_KEY[] = "PWR";
static const char RECIPE_TUNER_KEY[] = "TUNER";
static const char RECIPE_MFC_KEY[] = "MFC";
static const char RECIPE_MFC1_KEY[] = "MFC1";
static const char RECIPE_MFC2_KEY[] = "MFC2";
static const char RECIPE_MFC3_KEY[] = "MFC3";
static const char RECIPE_MFC4_KEY[] = "MFC4";
static const char RECIPE_MFC5_KEY[] = "MFC5";
static const char RECIPE_MFC6_KEY[] = "MFC6";
static const char RECIPE_THICKNESS_KEY[] = "THICKNESS";
static const char RECIPE_GAP_KEY[] = "GAP";
static const char RECIPE_OVERLAP_KEY[] = "OVERLAP";
static const char RECIPE_SPEED_KEY[] = "SPEED";
static const char RECIPE_CYCLES_KEY[] = "CYCLES";
static const char RECIPE_XMIN_KEY[] = "XMIN";
static const char RECIPE_YMIN_KEY[] = "YMIN";
static const char RECIPE_XMAX_KEY[] = "XMAX";
static const char RECIPE_YMAX_KEY[] = "YMAX";
static const char RECIPE_PURGE_KEY[] = "PURGE";
static const char RECIPE_AUTOSCAN_KEY[] = "AUTOSCAN";
static const char RECIPE_HEATER_KEY[] = "HEATER";
static const char RECIPE_DIRECTORY[] = "/opt/OTT_PLUS/Recipes";
static const char CASCADE_RECIPE_DIRECTORY[] = "/opt/OTT_PLUS/Cascade Recipes";

//const QString LOG_DIRECTORY = "/opt/OTT_PLUS/logs/";

//// settings keys
//const QString COLLISION_SYSTEM_ENABLED_SETTING = "/toolSettings/collisionSystemEnabled";
//const QString HEATER_ENABLED_SETTING = "/toolSettings/heaterEnabled";
//const QString BATCHID_ENABLED_SETTING = "/toolSettings/batchIDEnabled";

//// recipe key
//const QString RECIPE_PWR_KEY = "PWR";
//const QString RECIPE_TUNER_KEY = "TUNER";
//const QString RECIPE_MFC_KEY = "MFC";
//const QString RECIPE_MFC1_KEY = "MFC1";
//const QString RECIPE_MFC2_KEY = "MFC2";
//const QString RECIPE_MFC3_KEY = "MFC3";
//const QString RECIPE_MFC4_KEY = "MFC4";
//const QString RECIPE_MFC5_KEY = "MFC5";
//const QString RECIPE_MFC6_KEY = "MFC6";
//const QString RECIPE_THICKNESS_KEY = "THICKNESS";
//const QString RECIPE_GAP_KEY = "GAP";
//const QString RECIPE_OVERLAP_KEY = "OVERLAP";
//const QString RECIPE_SPEED_KEY = "SPEED";
//const QString RECIPE_CYCLES_KEY = "CYCLES";
//const QString RECIPE_XMIN_KEY = "XMIN";
//const QString RECIPE_YMIN_KEY = "YMIN";
//const QString RECIPE_XMAX_KEY = "XMAX";
//const QString RECIPE_YMAX_KEY = "YMAX";
//const QString RECIPE_PURGE_KEY = "PURGE";
//const QString RECIPE_AUTOSCAN_KEY = "AUTOSCAN";
//const QString RECIPE_HEATER_KEY = "HEATER";
//const QString RECIPE_DIRECTORY = "/opt/OTT_PLUS/Recipes";
//const QString CASCADE_RECIPE_DIRECTORY = "/opt/OTT_PLUS/Cascade Recipes";


#endif // UTILITIESANDCONSTANTS_H
