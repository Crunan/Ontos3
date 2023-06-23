#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "include/mainloop.h"
#include "include/commandfilereader.h"
#include "include/settingsdialog.h"
#include "include/recipe.h"
#include "include/logger.h"
#include "include/serialcomms.h"
#include "include/plasmacontroller/plasmacontroller.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <memory>
#include <QInputDialog>
#include <QLineEdit>


namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(MainLoop& loop, Logger& logger, QWidget *parent = nullptr);
    ~MainWindow();

    Recipe* getRecipe() const;
    void createRecipe();

private:
    // Reference to the MainLoop object
    MainLoop& mainLoop;
    // Reference to the logger
    Logger& log;
    Ui::MainWindow* ui = nullptr;
    QLabel* status = nullptr;
    SettingsDialog* settings = nullptr;
    // Pointer to Serial Communications
    std::shared_ptr<SerialComms> serial;
    // Pointer to the Recipe object
    Recipe* recipe;
    PlasmaController CTL;
    CommandFileReader commandFileReader;

    // Action Button methods
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    void about();
    void shutDownProgram();
    // Serial Port methodsRecipe*
    void handleSerialPortError();

    // Recipe methods
    //void createRecipe();
    //Recipe* getRecipe() const;


public slots:

    void openRecipeWindowMFC1();
};
#endif // MAINWINDOW_H
