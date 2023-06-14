QT       += core gui widgets serialport

greaterThan(QT_MAJOR_VERSION, 4)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line. DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    axis.cpp \
    axiscommands.cpp \
    axiscontroller.cpp \
    configuration.cpp \
    coordinate.cpp \
    functions.cpp \
    lighttowerstatemachine.cpp \
    logger.cpp \
    main.cpp \
    mainloop.cpp \
    mainstatemachine.cpp \
    mainwindow.cpp \
    mfc.cpp \
    plasmacontrollercommands.cpp \
    plasmahead.cpp \
    plasmasystemcontroller.cpp \
    pwr.cpp \
    recipe.cpp \
    scanstatemachine.cpp \
    serialcomms.cpp \
    settingsdialog.cpp \
    stage.cpp \
    stagemaneuvers.cpp \
    stagestatemachine.cpp \
    tuner.cpp

HEADERS += \
    axis.h \
    axiscommands.h \
    axiscontroller.h \
    configuration.h \
    coordinate.h \
    functions.h \
    lighttowerstatemachine.h \
    logger.h \
    mainloop.h \
    mainstatemachine.h \
    mainwindow.h \
    mfc.h \
    plasmacontrollercommands.h \
    plasmahead.h \
    plasmasystemcontroller.h \
    pwr.h \
    recipe.h \
    scanstatemachine.h \
    serialcomms.h \
    settingsdialog.h \
    stage.h \
    stagemaneuvers.h \
    stagestatemachine.h \
    tuner.h \
    ui_settingsdialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    ONTOS3.qrc

DISTFILES += \
    settingsdialog.o
