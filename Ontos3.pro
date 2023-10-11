QT += core gui widgets serialport statemachine

VERSION = 01.00.00
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

greaterThan(QT_MAJOR_VERSION, 4)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/diameter.cpp \
    src/serialinterface.cpp \
    src/axescontroller.cpp \
    src/axis.cpp \
    src/commandfilereader.cpp \
    src/commandmap.cpp \
    src/configuration.cpp \
    src/console.cpp \
    src/filereader.cpp \
    src/grblcontroller.cpp \
    src/led.cpp \
    src/ledfactory.cpp \
    src/ledstatus.cpp \
    src/lighttowerstatemachine.cpp \
    src/logger.cpp \
    src/main.cpp \
    src/mainloop.cpp \
    src/mainwindow.cpp \
    src/mfc.cpp \
    src/plasmacontroller.cpp \
    src/plasmahead.cpp \
    src/pwr.cpp \
    src/plasmarecipe.cpp \
    src/tuner.cpp \
    src/settingsdialog.cpp \
    src/stage.cpp


HEADERS += \
    src/diameter.h \
    src/axescontroller.h \
    src/axis.h \
    src/commandfilereader.h \
    src/commandmap.h \
    src/configuration.h \
    src/console.h \
    src/diameter.h \
    src/filereader.h \
    src/grblcontroller.h \
    src/led.h \
    src/ledfactory.h \
    src/ledstatus.h \
    src/lighttowerstatemachine.h \
    src/logger.h \
    src/mainwindow.h \
    src/mainloop.h \
    src/mfc.h \
    src/plasmacontroller.h \
    src/plasmahead.h \
    src/pwr.h \
    src/plasmarecipe.h \
    src/tuner.h \
    src/settingsdialog.h \
    src/serialinterface.h \
    src/stage.h

FORMS += \
    src/mainwindow.ui \
    src/settingsdialog.ui

RESOURCES += \
    src/ONTOS3.qrc

#INCLUDEPATH += $$PWD/include

