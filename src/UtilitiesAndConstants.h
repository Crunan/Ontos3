#ifndef UTILITIESANDCONSTANTS_H
#define UTILITIESANDCONSTANTS_H

#include <QString>

bool isBitSet(int test_int, int bit_pos);

// logging
const QString LOG_DIRECTORY = "/opt/OTT_PLUS/logs/";

// settings keys
const QString COLLISION_SYSTEM_ENABLED_SETTING = "/toolSettings/collisionSystemEnabled";
const QString HEATER_ENABLED_SETTING = "/toolSettings/heaterEnabled";
const QString BATCHID_ENABLED_SETTING = "/toolSettings/batchIDEnabled";

// recipe key
const QString RECIPE_PWR_KEY = "PWR";
const QString RECIPE_TUNER_KEY = "TUNER";
const QString RECIPE_MFC_KEY = "MFC";
const QString RECIPE_MFC1_KEY = "MFC1";
const QString RECIPE_MFC2_KEY = "MFC2";
const QString RECIPE_MFC3_KEY = "MFC3";
const QString RECIPE_MFC4_KEY = "MFC4";
const QString RECIPE_THICKNESS_KEY = "THICKNESS";
const QString RECIPE_GAP_KEY = "GAP";
const QString RECIPE_OVERLAP_KEY = "OVERLAP";
const QString RECIPE_SPEED_KEY = "SPEED";
const QString RECIPE_CYCLES_KEY = "CYCLES";
const QString RECIPE_XMIN_KEY = "XMIN";
const QString RECIPE_YMIN_KEY = "YMIN";
const QString RECIPE_XMAX_KEY = "XMAX";
const QString RECIPE_YMAX_KEY = "YMAX";
const QString RECIPE_PURGE_KEY = "PURGE";
const QString RECIPE_AUTOSCAN_KEY = "AUTOSCAN";
const QString RECIPE_HEATER_KEY = "HEATER";
const QString RECIPE_DIRECTORY = "/opt/OTT_PLUS/Recipes";


#endif // UTILITIESANDCONSTANTS_H
