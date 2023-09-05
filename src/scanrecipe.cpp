#include "scanrecipe.h"

ScanRecipe::ScanRecipe(QObject* parent) : QObject(parent) {
    // Initialize member variables here
}

ScanRecipe::~ScanRecipe() {
    // Cleanup any resources here
}

double ScanRecipe::getSubstrateThickness() const {
    return substrateThickness_;
}

void ScanRecipe::setSubstrateThickness(double thickness) {
    substrateThickness_ = thickness;
    emit substrateThicknessChanged();
}

double ScanRecipe::getPlasmaHeadGap() const {
    return plasmaHeadGap_;
}

void ScanRecipe::setPlasmaHeadGap(double gap) {
    plasmaHeadGap_ = gap;
    emit plasmaHeadGapChanged();
}

double ScanRecipe::getSubstrateOverlap() const {
    return substrateOverlap_;
}

void ScanRecipe::setSubstrateOverlap(double overlap) {
    substrateOverlap_ = overlap;
    emit substrateOverlapChanged();
}

double ScanRecipe::getStageSpeed() const {
    return stageSpeed_;
}

void ScanRecipe::setStageSpeed(double speed) {
    stageSpeed_ = speed;
    emit stageSpeedChanged();
}

int ScanRecipe::getNumCycles() const {
    return numCycles_;
}

void ScanRecipe::setNumCycles(int cycles) {
    numCycles_ = cycles;
    emit numCyclesChanged();
}

bool ScanRecipe::isAutoScanOn() const {
    return autoScanOn_;
}

void ScanRecipe::setAutoScanOn(bool enabled) {
    autoScanOn_ = enabled;
    emit autoScanOnChanged();
}

bool ScanRecipe::isN2PurgeOn() const {
    return N2PurgeOn_;
}

void ScanRecipe::setN2PurgeOn(bool enabled) {
    N2PurgeOn_ = enabled;
    emit N2PurgeOnChanged();
}
