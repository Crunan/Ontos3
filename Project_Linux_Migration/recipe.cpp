#include "recipe.h"

Recipe::Recipe(QObject* parent) : QObject(parent) {
    // Initialize member variables with default values
    substrateThickness = 0.0;
    plasmaHeadGap = 0.0;
    substrateOverlap = 0.0;
    stageSpeed = 0.0;
    numCycles = 1;
    autoScanOn = false;
    N2PurgeOn = false;
}

Recipe::~Recipe() {}

double Recipe::getSubstrateThickness() const {
    return substrateThickness;
}

void Recipe::setSubstrateThickness(double thickness) {
    if (substrateThickness != thickness) {
        substrateThickness = thickness;
        emit substrateThicknessChanged();
    }
}

double Recipe::getPlasmaHeadGap() const {
    return plasmaHeadGap;
}

void Recipe::setPlasmaHeadGap(double gap) {
    if (plasmaHeadGap != gap) {
        plasmaHeadGap = gap;
        emit plasmaHeadGapChanged();
    }
}

double Recipe::getSubstrateOverlap() const {
    return substateOverlap;
}

void Recipe::setSubstrateOverlap(double overlap) {
    if (substateOverlap != overlap) {
        substateOverlap = overlap;
        emit substrateOverlapChanged();
    }
}

double Recipe::getStageSpeed() const {
    return stageSpeed;
}

void Recipe::setStageSpeed(double speed) {
    if (stageSpeed != speed) {
        stageSpeed = speed;
        emit stageSpeedChanged();
    }
}

int Recipe::getNumCycles() const {
    return numCycles;
}

void Recipe::setNumCycles(int cycles) {
    if (numCycles != cycles) {
        numCycles = cycles;
        emit numCyclesChanged();
    }
}

bool Recipe::isAutoScanOn() const {
    return autoScanOn;
}

void Recipe::setAutoScanOn(bool enabled) {
    if (autoScanOn != enabled) {
        autoScanOn = enabled;
        emit autoScanOnChanged();
    }
}

bool Recipe::isN2PurgeOn() const {
    return N2PurgeOn;
}

void Recipe::setN2PurgeOn(bool enabled) {
    if (N2PurgeOn != enabled) {
        N2PurgeOn = enabled;
        emit N2PurgeOnChanged();
    }
}

void MainWindow::saveCascadeFile() {
    QStringList StrVar;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Current Cascade Recipe"), "/home/oem/Ontos3/Project_Linux_Migration/cascade_recipes/",
                                                    tr("Recipe List (*.crcp);;All Files (*)"));
    if (!fileName.contains(".crcp")) {fileName += ".crcp";}
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;

        }
        QTextStream out(&file);
        for(int i = 0; i > ui->cascadeList->count(); i++) {
            out<<ui->cascadeList->item(i);
        }
        file.close();
        StrVar = fileName.split("/");
        ui->cascade_recipe_name->setText(StrVar.takeLast());
    }
}
void MainWindow::loadCascadeFile() {
    QStringList cascade_recipe;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Recipe from File"), "/home/oem/Ontos3/Project_Linux_Migration/cascade_recipes/",
                                                    tr("Recipe File (*.crcp);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            cascade_recipe += line;
        }
        file.close();

        ui->cascadeList->addItems(cascade_recipe);
    }
}
void MainWindow::saveToFile() {
    QStringList StrVar;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Current Recipe"), "/home/oem/Ontos3/Project_Linux_Migration/recipes/",
                                                    tr("Recipe List (*.rcp);;All Files (*)"));
    if (!fileName.contains(".rcp")) {fileName += ".rcp";}
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;

        }
        QString RecipeString = "MFC1:" + MFC[1].getLoadedFlowQStr() + '\n' +
                               "MFC2:" + MFC[2].getLoadedFlowQStr() + '\n' +
                               "MFC3:" + MFC[3].getLoadedFlowQStr() + '\n' +
                               "MFC4:" + MFC[4].getLoadedFlowQStr() + '\n' +
                               "PWR:" + RF.getLoadedSetPointQStr() + '\n' +
                               "TUNER:" + TUNER.getLoadedSPQStr() + '\n' +
                               "AUTOTUNE:" + TUNER.getAutoTuneQStr() + '\n' +
                               "THICKNESS:" + recipe.getThickness() + '\n' +
                               "GAP:" + recipe.getGap() + '\n' +
                               "OVERLAP:" + recipe.getOverlap() + '\n' +
                               "SPEED:" + recipe.getSpeed() + '\n' +
                               "CYCLES:" + recipe.getCycles() + '\n' +
                               "X1:" + ScanSM.getX1String() + '\n' +
                               "X2:" + ScanSM.getX2String() + '\n' +
                               "Y1:" + ScanSM.getY1String() + '\n' +
                               "Y2:" + ScanSM.getY2String() + '\n' +
                               "PURGE:" + recipe.getPurgeQStr() + '\n' +
                               "AUTOSCAN:" + recipe.getAutoScan();


        QTextStream out(&file);
        logInfo("Saved " + fileName + " : " + RecipeString);
        out << RecipeString.toUtf8();
        file.close();
        StrVar = fileName.split("/");
        ui->name_recipe->setText(StrVar.takeLast());
    }
}

void MainWindow::loadFromFile() {
    QStringList recipe;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Recipe from File"), "/home/oem/Ontos3/Project_Linux_Migration/recipes/",
                                                    tr("Recipe File (*.rcp);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            recipe += line;
        }
        recipe += "NAME:" + file.fileName();
        file.close();
        loadGUI(recipe);
        showStatusMessage(tr("Loaded Recipe"));
    }
}

