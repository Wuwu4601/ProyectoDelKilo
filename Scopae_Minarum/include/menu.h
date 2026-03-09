#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
  Rectangle rect;
  const char *text;
  bool hovered;
  bool pressed;
} Button;

Button createButton(int x, int y, int width, int height, const char* text);
int checkButtonPress(Button* btn);
void drawButton(Button* btn, Color normalColor, Color hoverColor);

void initMenu();
void updateMenu();
void drawMenu();
void updateStats();
Button createButton(int x, int y, int width, int height, const char *text);
int checkButtonPress(Button *btn);

#endif
