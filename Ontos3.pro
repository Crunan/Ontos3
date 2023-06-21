QT       += core gui widgets serialport

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
    src/axiscontroller/stage/positions.cpp \
    src/axiscontroller/stage/statemachine.cpp \
    src/commandfilereader.cpp \
    src/configuration.cpp \
    src/axiscontroller/coordinate.cpp \
    src/controllerinterface.cpp \
    src/functions.cpp \
    src/lighttowerstatemachine.cpp \
    src/logger.cpp \
    src/main.cpp \
    src/mainloop.cpp \
    src/mainstatemachine.cpp \
    src/mainwindow.cpp \
    src/axiscontroller/stage/maneuvers.cpp \
    src/plasmacontroller/mfc.cpp \
    src/plasmacontroller/plasmacontroller.cpp \
    src/plasmacontroller/plasmahead.cpp \
    src/plasmacontroller/pwr.cpp \
    src/recipe.cpp \
    src/axiscontroller/stage/scan.cpp \
    src/serialcomms.cpp \
    src/settingsdialog.cpp \
    src/plasmacontroller/tuner.cpp \
    src/axiscontroller/stage/twospot.cpp

HEADERS += \
    include/axiscontroller/axiscommands.h \
    include/axiscontroller/axiscontroller.h \
    include/axiscontroller/stage/StageNamespace.h \
    include/axiscontroller/axis.h \
    include/axiscontroller/stage/diameter.h \
    include/axiscontroller/stage/load.h \
    include/axiscontroller/stage/positions.h \
    include/commandfilereader.h \
    include/commandhandler.h \
    include/commandhandlerfactory.h \
    include/commandmap.h \
    include/configuration.h \
    include/controllerinterface.h \
    include/axiscontroller/coordinate.h \
    include/functions.h \
    include/lighttowerstatemachine.h \
    include/logger.h \
    include/mainwindow.h \
    include/mainloop.h \
    include/axiscontroller/stage/maneuvers.h \
    include/plasmacontroller/mfc.h \
    include/plasmacontroller/plasmacontroller.h \
    include/plasmacontroller/plasmahead.h \
    include/plasmacontroller/pwr.h \
    include/recipe.h \
    include/axiscontroller/stage/scan.h \
    include/serialcomms.h \
    include/settingsdialog.h \
    include/axiscontroller/stage/statemachine.h \
    include/plasmacontroller/tuner.h \
    include/axiscontroller/stage/initialize.h \
    include/axiscontroller/stage/twospot.h \



FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    ONTOS3.qrc

DISTFILES += \
    settingsdialog.o

INCLUDEPATH += $$PWD/include

