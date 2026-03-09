#include "raylib.h"
#include "../include/game.h"
#include "../include/save.h"
#include "../include/menu.h"
#include "../include/config.h"

Button btnPlay, btnStats, btnQuit, btnBack;
#define MENU_TITLE_Y 150
#define BTN_WIDTH 300
#define BTN_HEIGHT 60

void initMenu()
{
    int startX = (SCREEN_WIDTH - BTN_WIDTH) / 2;
    btnPlay = createButton(startX, MENU_TITLE_Y + 100, BTN_WIDTH, BTN_HEIGHT, "PLAY");
    btnStats = createButton(startX, MENU_TITLE_Y + 180, BTN_WIDTH, BTN_HEIGHT, "STATS");
    btnQuit = createButton(startX, MENU_TITLE_Y + 260, BTN_WIDTH, BTN_HEIGHT, "QUIT");
    btnBack = createButton(startX, SCREEN_HEIGHT - 120, BTN_WIDTH, BTN_HEIGHT, "BACK");
}

Button createButton(int x, int y, int width, int height, const char* text)
{
    Button btn;
    btn.rect = (Rectangle){ (float)x, (float)y, (float)width, (float)height };
    btn.text = text;
    btn.hovered = 0;
    btn.pressed = 0;
    return btn;
}

int checkButtonPress(Button* btn)
{
    Vector2 mouse;
    mouse = GetMousePosition();
    btn->hovered = CheckCollisionPointRec(mouse, btn->rect);
    if (btn->hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        btn->pressed = 1;
        return 1;
    }
    return 0;
}


void startNewGame()
{
    
    calicataLevel = 1;
    money = 0;
    sondeoBeaten = 0;
    cataBeaten = 0;
    calicataBeaten = 0;
    roundNumber = 1;
    roundType = ROUND_CATITO;
    currentLives = maxLives;
    score = 0;
    baseScore = 0;
    totalScore = 0;
    timeBonus = 0;
    roundMoney = 0;
    roundCompleted = 0;
    gameTime = 0;
    gameOver = 0;
    gameWon = 0;
    
   
    save.totalMoney = 0;
    save.runsCompleted = 0;
   
    
    saveGame();
    
  
    initGame();
    gameState = STATE_PLAYING;
}

void updateMenu()
{
    if (IsKeyPressed(KEY_ESCAPE)) return;
    
    if (checkButtonPress(&btnPlay)) {
        startNewGame();  
    }
    if (checkButtonPress(&btnStats)) {
        gameState = STATE_STATS;
    }
    if (checkButtonPress(&btnQuit)) {
        CloseWindow();
    }
}

void updateStats()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        gameState = STATE_MENU;
        return;
    }
    if (checkButtonPress(&btnBack)) {
        gameState = STATE_MENU;
    }
}

void drawButton(Button* btn, Color normalColor, Color hoverColor)
{
    Color color;
    int textWidth, textX, textY;
    
    color = btn->hovered ? hoverColor : normalColor;
    DrawRectangleRec(btn->rect, color);
    DrawRectangleLinesEx(btn->rect, 3, WHITE);
    textWidth = MeasureText(btn->text, 30);
    textX = (int)btn->rect.x + (btn->rect.width - textWidth) / 2;
    textY = (int)btn->rect.y + (btn->rect.height - 30) / 2;
    DrawText(btn->text, textX, textY, 30, BLACK);
}

void drawMenu()
{
    DrawText("SCOPAE MINARUM", SCREEN_WIDTH/2 - 250, MENU_TITLE_Y, 60, GOLD);
    
    drawButton(&btnPlay, (Color){255, 200, 100, 255}, (Color){255, 220, 150, 255});
    drawButton(&btnStats, (Color){100, 200, 255, 255}, (Color){150, 220, 255, 255});
    drawButton(&btnQuit, (Color){255, 100, 100, 255}, (Color){255, 150, 150, 255});
}

void drawStats()
{
    int startY, spacing;
    
    DrawText("STATISTICS", SCREEN_WIDTH/2 - 150, MENU_TITLE_Y, 60, SKYBLUE);
    
    startY = MENU_TITLE_Y + 120;
    spacing = 45;
    
    DrawText(TextFormat("Total Mines Found: %i", save.totalMinesFound), SCREEN_WIDTH/2 - 150, startY, 26, RED);
    DrawText(TextFormat("Total Tiles Revealed: %i", save.totalTilesRevealed), SCREEN_WIDTH/2 - 150, startY + spacing, 26, GREEN);
    DrawText(TextFormat("Runs Completed: %i", save.runsCompleted), SCREEN_WIDTH/2 - 150, startY + spacing*2, 26, GOLD);
    DrawText(TextFormat("Best Calicata Level: %i", save.bestCalicata), SCREEN_WIDTH/2 - 150, startY + spacing*3, 26, ORANGE);
    DrawText(TextFormat("Best Score: %i", save.bestScore), SCREEN_WIDTH/2 - 150, startY + spacing*4, 26, PURPLE);
    
    drawButton(&btnBack, (Color){150, 150, 150, 255}, (Color){200, 200, 200, 255});
    DrawText("Press ESC to go back", SCREEN_WIDTH/2 - 90, SCREEN_HEIGHT - 40, 18, GRAY);
}