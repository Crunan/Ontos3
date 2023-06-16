QT       += core gui widgets serialport

greaterThan(QT_MAJOR_VERSION, 4)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/axiscontroller/axis.cpp \
    src/axiscontroller/commands.cpp \
    src/axiscontroller/controller.cpp \
    src/axiscontroller/stage/diameter.cpp \
    src/axiscontroller/stage/initialize.cpp \
    src/axiscontroller/stage/load.cpp \
    src/axiscontroller/stage/positions.cpp \
    src/axiscontroller/stage/statemachine.cpp \
    src/configuration.cpp \
    src/axiscontroller/coordinate.cpp \
    src/functions.cpp \
    src/lighttowerstatemachine.cpp \
    src/logger.cpp \
    src/main.cpp \
    src/mainloop.cpp \
    src/mainstatemachine.cpp \
    src/mainwindow.cpp \
    src/axiscontroller/stage/maneuvers.cpp \
    src/plasmacontroller/commands.cpp \
    src/plasmacontroller/controller.cpp \
    src/plasmacontroller/mfc.cpp \
    src/plasmacontroller/plasmahead.cpp \
    src/plasmacontroller/pwr.cpp \
    src/recipe.cpp \
    src/axiscontroller/stage/scan.cpp \
    src/serialcomms.cpp \
    src/settingsdialog.cpp \
    src/plasmacontroller/tuner.cpp \
    src/axiscontroller/stage/twospot.cpp

HEADERS += \
    include/axiscontroller/AxisControllerNamespace.h \
    include/axiscontroller/commands.h \
    include/axiscontroller/controller.h \
    include/axiscontroller/stage/StageNamespace.h \
    include/axiscontroller/axis.h \
    include/axiscontroller/stage/diameter.h \
    include/axiscontroller/stage/load.h \
    include/axiscontroller/stage/positions.h \
    include/configuration.h \
    include/axiscontroller/coordinate.h \
    include/functions.h \
    include/lighttowerstatemachine.h \
    include/logger.h \
    include/mainwindow.h \
    include/axiscontroller/stage/maneuvers.h \
    include/plasmacontroller/PlasmaControllerNamespace.h \
    include/plasmacontroller/commands.h \
    include/plasmacontroller/controller.h \
    include/plasmacontroller/mfc.h \
    include/plasmacontroller/plasmahead.h \
    include/plasmacontroller/pwr.h \
    include/recipe.h \
    include/axiscontroller/stage/scan.h \
    include/serialcomms.h \
    include/settingsdialog.h \
    include/axiscontroller/stage/statemachine.h \
    include/plasmacontroller/tuner.h \
    include/ui_settingsdialog.h \
    src/axiscontroller/stage/initialize.h \
    src/axiscontroller/stage/twospot.h

FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    ONTOS3.qrc

DISTFILES += \
    settingsdialog.o

INCLUDEPATH += $$PWD/include

