QT       += widgets serialport

greaterThan(QT_MAJOR_VERSION, 4)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line. DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp

HEADERS += \
    logger.h \
    mainwindow.h \
    settingsdialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    ATMOSGUI.qrc

DISTFILES +=
