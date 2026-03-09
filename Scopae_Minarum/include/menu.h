#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
    Rectangle rect;
    const char* text;
    int hovered;
    int pressed;
} Button;

Button createButton(int x, int y, int width, int height, const char* text);
int checkButtonPress(Button* btn);
void drawButton(Button* btn, Color normalColor, Color hoverColor);

void initMenu();
void updateMenu();
void drawMenu();
void updateStats();
void drawStats();
void startNewGame(); 

#endif