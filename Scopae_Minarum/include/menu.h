#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
    Rectangle rect;
    const char* text;
    bool hovered;
    bool pressed;
} Button;

void initMenu();
void updateMenu();
void drawMenu();
void drawStats();
void updateStats();
Button createButton(int x, int y, int width, int height, const char* text);
bool checkButtonPress(Button* btn);

#endif