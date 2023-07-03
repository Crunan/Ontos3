QT += core gui widgets serialport

greaterThan(QT_MAJOR_VERSION, 4)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/axiscontroller/axis.cpp \
    src/axiscontroller/axiscommands.cpp \
    src/axiscontroller/axiscontroller.cpp \
    src/axiscontroller/stage/diameter.cpp \
    src/axiscontroller/stage/initialize.cpp \
    src/axiscontroller/stage/load.cpp \
    src/axiscontroller/stage/maneuvers.cpp \
    src/axiscontroller/stage/positions.cpp \
    src/axiscontroller/stage/scan.cpp \
    src/axiscontroller/scanrecipe.cpp \
    src/axiscontroller/stage/statemachine.cpp \
    src/axiscontroller/stage/twospot.cpp \
    src/commandfilereader.cpp \
    src/commandmap.cpp \
    src/configuration.cpp \
    src/axiscontroller/coordinate.cpp \
    src/functions.cpp \
    src/led.cpp \
    src/ledfactory.cpp \
    src/lighttowerstatemachine.cpp \
    src/logger.cpp \
    src/main.cpp \
    src/mainloop.cpp \
    src/mainstatemachine.cpp \
    src/mainwindow.cpp \
    src/plasmacontroller/mfc.cpp \
    src/plasmacontroller/plasmacontroller.cpp \
    src/plasmacontroller/plasmahead.cpp \
    src/plasmacontroller/pwr.cpp \
    src/plasmacontroller/plasmarecipe.cpp \
    src/plasmacontroller/tuner.cpp \
    src/recipe.cpp \
    src/serialportmanager.cpp \
    src/serialcommandhandler.cpp \
    src/settingsdialog.cpp


HEADERS += \
    include/axiscontroller/axis.h \
    include/axiscontroller/axiscommands.h \
    include/axiscontroller/axiscontroller.h \
    include/axiscontroller/coordinate.h \
    include/axiscontroller/stage/diameter.h \
    include/axiscontroller/stage/initialize.h \
    include/axiscontroller/stage/load.h \
    include/axiscontroller/stage/maneuvers.h \
    include/axiscontroller/stage/positions.h \
    include/axiscontroller/stage/scan.h \
    include/axiscontroller/scanrecipe.h \
    include/axiscontroller/stage/statemachine.h \
    include/axiscontroller/stage/twospot.h \
    include/commandfilereader.h \
    include/commandmap.h \
    include/configuration.h \
    include/functions.h \
    include/led.h \
    include/ledfactory.h \
    include/lighttowerstatemachine.h \
    include/logger.h \
    include/mainwindow.h \
    include/mainloop.h \
    include/plasmacontroller/mfc.h \
    include/plasmacontroller/plasmacontroller.h \
    include/plasmacontroller/plasmahead.h \
    include/plasmacontroller/pwr.h \
    include/plasmacontroller/plasmarecipe.h \
    include/plasmacontroller/tuner.h \
    include/recipe.h \
    include/serialportmanager.h \
    include/settingsdialog.h \
    include/serialcommandhandler.h


FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    ONTOS3.qrc

INCLUDEPATH += $$PWD/include

