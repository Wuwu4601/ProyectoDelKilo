#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stdbool.h>

void drawSidebar();
void drawBottomBar();
void drawBoard(int offsetX, int offsetY, int tileSize);
void updateUI();
bool checkOptionsButtonPress();
bool checkMenuButtonPress(int btnX, int btnY, int btnWidth, int btnHeight);
bool checkResumeButtonPress(int btnX, int btnY, int btnWidth, int btnHeight);
void drawCataSelector();
void drawCataSelectorSidebar();
bool checkCataSelectPress();
bool checkSkipSondeoPress();
bool checkSkipCataPress();
void drawEndOfRound();  // ✅ Declaración

#endif