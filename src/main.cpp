#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "mainloop.h"
#include "logger.h"

#include "ui_mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

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
    window.show();

    //end program
    bool state = a.exec();
    return state;
}

