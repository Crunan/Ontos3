#pragma once
#include <QString>

void WriteLogLine(QString st_Line);

void UpdateStatus(int myStatusBits);

void RunCheckInput();

void RunPolling();

void OpenLogFile();

void RunInitAxesSM();

void RunTwoSpotSM();

void RunScanSM();

void RunHomeAxesSM();

void RunCollisionPassSM();

void CollisionLaser();

void SetLightTower();

void RunShutDown();


