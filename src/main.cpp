#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "mainloop.h"
#include "logger.h"

#include "ui_mainwindow.h"
#include <qtsingleapplication.h>


int main(int argc, char *argv[]) {
    //QApplication a(argc, argv);
    QtSingleApplication app("OntosPlasma", argc, argv);

    if (app.isRunning())
        return 0;

    // these allow us to use the QSettings from multiple places in the app
    // using the default constructor
    QCoreApplication::setOrganizationName("OntosPlasma");
    QCoreApplication::setOrganizationDomain("OntosPlasma.com");
    QCoreApplication::setApplicationName("Ontos3");

    // Create our main loop
    // Contains our Event loop and Timer
    MainLoop *mainLoop = new MainLoop(); // cleaned up in MainWindow destructor
    mainLoop->start();

    // Initialize our logger
    Logger::init();

    Logger::logInfo("Current application version: " + QString(APP_VERSION));

    // Create our main window
    // Contains our GUI
    MainWindow window(mainLoop);
    app.setActivationWindow(&window);
    window.show();

    bool state = app.exec();
    return state;
}

