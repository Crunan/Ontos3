QT += core gui widgets serialport statemachine



greaterThan(QT_MAJOR_VERSION, 4)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/diameter.cpp \
    src/serialinterface.cpp \
    src/axescontroller/axescontroller.cpp \
    src/axescontroller/axis.cpp \
    src/axescontroller/axiscommands.cpp \
    src/axescontroller/axiscontroller.cpp \
    src/axescontroller/coordinate.cpp \
    src/axescontroller/stage/initialize.cpp \
    src/axescontroller/stage/load.cpp \
    src/axescontroller/stage/maneuvers.cpp \
    src/axescontroller/stage/positions.cpp \
    src/axescontroller/stage/scan.cpp \
    src/axescontroller/scanrecipe.cpp \
    src/axescontroller/stage/statemachine.cpp \
    src/axescontroller/stage/twospot.cpp \
    src/commandfilereader.cpp \
    src/commandmap.cpp \
    src/configuration.cpp \
    src/console.cpp \
    src/filereader.cpp \
    src/functions.cpp \
    src/grblcontroller.cpp \
    src/led.cpp \
    src/ledfactory.cpp \
    src/ledstatus.cpp \
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
    src/settingsdialog.cpp \
    src/stage.cpp \
    src/stagewidget.cpp


HEADERS += \
    include/diameter.h \
    include/axescontroller/axescontroller.h \
    include/axescontroller/axis.h \
    include/axescontroller/axiscommands.h \
    include/axescontroller/axiscontroller.h \
    include/axescontroller/coordinate.h \
    include/axescontroller/stage/initialize.h \
    include/axescontroller/stage/load.h \
    include/axescontroller/stage/maneuvers.h \
    include/axescontroller/stage/positions.h \
    include/axescontroller/stage/scan.h \
    include/axescontroller/scanrecipe.h \
    include/axescontroller/stage/statemachine.h \
    include/axescontroller/stage/twospot.h \
    include/commandfilereader.h \
    include/commandmap.h \
    include/configuration.h \
    include/console.h \
    include/diameter.h \
    include/filereader.h \
    include/functions.h \
    include/grblcontroller.h \
    include/led.h \
    include/ledfactory.h \
    include/ledstatus.h \
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
    include/settingsdialog.h \
    include/stagewidget.h \
    include/serialinterface.h \
    include/axescontroller/zaxis.h \
    include/stage.h

FORMS += \
    ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    ONTOS3.qrc

INCLUDEPATH += $$PWD/include

