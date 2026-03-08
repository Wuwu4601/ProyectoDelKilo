#ifndef UI_H
#define UI_H

#include "raylib.h"

void drawSidebar();
void drawBottomBar();
void drawBoard(int offsetX, int offsetY, int tileSize);
void updateUI();
int checkOptionsButtonPress();
int checkMenuButtonPress(int btnX, int btnY, int btnWidth, int btnHeight);
int checkResumeButtonPress(int btnX, int btnY, int btnWidth, int btnHeight);
void drawCataSelector();
void drawCataSelectorSidebar();
int checkCataSelectPress();
int checkSkipSondeoPress();
int checkSkipCataPress();
void drawEndOfRound();

#endif
