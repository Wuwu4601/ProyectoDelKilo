#include "../include/board.h"
#include "../include/config.h"
#include "../include/game.h"
#include "../include/menu.h"
#include "../include/save.h"
#include "../include/ui.h"
#include "raylib.h"

int main() {
  SetExitKey(0);

  loadSave();
  initMenu();

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scopae Minarum");
  SetTargetFPS(60);

  int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;

  // ✅ TILE_SIZE dinámico según la ronda
  int currentTileSize = TILE_SIZE;
  if (roundType == ROUND_CATITO) {
    currentTileSize = TILE_SIZE_CATITO;
  }

  int boardPixelWidth = currentGridW * currentTileSize;
  int boardPixelHeight = currentGridH * currentTileSize;
  int offsetX = (sidebarX - boardPixelWidth) / 2;
  int offsetY = 50;

  int menuBtnWidth = 200;
  int menuBtnHeight = 50;
  int menuBtnX = sidebarX / 2 - menuBtnWidth / 2;
  int menuBtnY = SCREEN_HEIGHT / 2 + 100;

  int resumeBtnWidth = 200;
  int resumeBtnHeight = 50;
  int resumeBtnX = sidebarX / 2 - resumeBtnWidth / 2;
  int resumeBtnY = SCREEN_HEIGHT / 2 + 100;

  // ✅ menuTextWidth declarado AQUÍ (al inicio, para que funcione en todos los
  // case)
  int menuTextWidth = 0;

  int optionsOpen = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground((Color){15, 15, 20, 255});

    // ✅ Recalcular posición del tablero en cada frame
    currentTileSize = TILE_SIZE;
    if (roundType == ROUND_CATITO) {
      currentTileSize = TILE_SIZE_CATITO;
    }
    boardPixelWidth = currentGridW * currentTileSize;
    boardPixelHeight = currentGridH * currentTileSize;
    offsetX = (sidebarX - boardPixelWidth) / 2;
    offsetY = 50;

    switch (gameState) {
    case STATE_MENU: {
      updateMenu();
      drawMenu();
    } break;

    case STATE_STATS: {
      updateStats();
      drawStats();
    } break;

    case STATE_CATA_SELECT: {
      drawCataSelector();

      if (checkCataSelectPress()) {
        goToNextAvailable();
      }

      if (checkSkipSondeoPress()) {
        sondeoBeaten = 1;
      }

      if (checkSkipCataPress()) {
        cataBeaten = 1;
        roundType = ROUND_CALICATA;
        roundNumber++;
        initGame();
        currentLives = maxLives;
        gameState = STATE_PLAYING;
      }

      if (IsKeyPressed(KEY_C)) {
        goToNextAvailable();
      }
      if (IsKeyPressed(KEY_S) && sondeoBeaten && !cataBeaten) {
        cataBeaten = 1;
        roundType = ROUND_CALICATA;
        roundNumber++;
        initGame();
        currentLives = maxLives;
        gameState = STATE_PLAYING;
      }

    } break;

    case STATE_PLAYING: {
      if (IsKeyPressed(KEY_ESCAPE)) {
        gameState = STATE_PAUSE;
        break;
      }

      updateUI();
      updateGame();

      if (checkOptionsButtonPress() && !optionsOpen) {
        optionsOpen = 1;
        gameState = STATE_OPTIONS;
        break;
      }

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !gameOver) {
        Vector2 m = GetMousePosition();
        if (m.x >= offsetX && m.x < offsetX + boardPixelWidth &&
            m.y >= offsetY && m.y < offsetY + boardPixelHeight) {
          int x = (int)((m.x - offsetX) / currentTileSize);
          int y = (int)((m.y - offsetY) / currentTileSize);
          if (x >= 0 && x < currentGridW && y >= 0 && y < currentGridH) {
            if (!flagged[y][x])
              revealTile(x, y);
          }
        }
      }

      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 m = GetMousePosition();
        if (m.x >= offsetX && m.x < offsetX + boardPixelWidth &&
            m.y >= offsetY && m.y < offsetY + boardPixelHeight) {
          int x = (int)((m.x - offsetX) / currentTileSize);
          int y = (int)((m.y - offsetY) / currentTileSize);
          if (x >= 0 && x < currentGridW && y >= 0 && y < currentGridH) {
            toggleFlag(x, y);
          }
        }
      }

      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);

    } break;

    case STATE_PAUSE: {
      updateUI();
      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);
      DrawRectangle(0, 0, sidebarX, SCREEN_HEIGHT, (Color){0, 0, 0, 200});

      DrawText("PAUSED", sidebarX / 2 - 100, SCREEN_HEIGHT / 2 - 60, 60, WHITE);
      DrawText("Press ESC to resume", sidebarX / 2 - 110,
               SCREEN_HEIGHT / 2 + 10, 25, LIGHTGRAY);
      DrawText("Press M for Menu", sidebarX / 2 - 130, SCREEN_HEIGHT / 2 + 50,
               22, GRAY);

      DrawRectangle(resumeBtnX, resumeBtnY, resumeBtnWidth, resumeBtnHeight,
                    (Color){50, 150, 50, 255});
      DrawRectangleLines(resumeBtnX, resumeBtnY, resumeBtnWidth,
                         resumeBtnHeight, LIGHTGRAY);
      int resumeTextWidth = MeasureText("RESUME", 28);
      DrawText("RESUME", resumeBtnX + (resumeBtnWidth - resumeTextWidth) / 2,
               resumeBtnY + 13, 28, WHITE);

      if (IsKeyPressed(KEY_ESCAPE))
        gameState = STATE_PLAYING;
      if (IsKeyPressed(KEY_M))
        gameState = STATE_MENU;
      if (checkResumeButtonPress(resumeBtnX, resumeBtnY, resumeBtnWidth,
                                 resumeBtnHeight)) {
        gameState = STATE_PLAYING;
      }

    } break;

    case STATE_SHOP: {
      if (IsKeyPressed(KEY_ESCAPE)) {
        gameState = STATE_PAUSE;
        break;
      }

      updateUI();
      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);
      DrawRectangle(0, 0, sidebarX, SCREEN_HEIGHT, (Color){0, 0, 0, 200});

      DrawText("ROUND COMPLETE!", sidebarX / 2 - 200, SCREEN_HEIGHT / 2 - 40,
               50, GREEN);
      DrawText(TextFormat("MONEY: $%i", money), sidebarX / 2 - 80,
               SCREEN_HEIGHT / 2 + 20, 30, GOLD);
      DrawText("PRESS C TO CONTINUE", sidebarX / 2 - 140,
               SCREEN_HEIGHT / 2 + 70, 25, WHITE);

      if (IsKeyPressed(KEY_C))
        nextRound();

    } break;

    case STATE_GAMEOVER: {
      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);
      DrawRectangle(0, 0, sidebarX, SCREEN_HEIGHT, (Color){0, 0, 0, 200});

      DrawText("GAME OVER", sidebarX / 2 - 180, SCREEN_HEIGHT / 2 - 40, 60,
               RED);
      DrawText(TextFormat("FINAL SCORE: %i", score), sidebarX / 2 - 120,
               SCREEN_HEIGHT / 2 + 20, 30, GOLD);
      DrawText(TextFormat("CALICATA: %i", calicataLevel), sidebarX / 2 - 100,
               SCREEN_HEIGHT / 2 + 60, 25, WHITE);

      DrawRectangle(menuBtnX, menuBtnY, menuBtnWidth, menuBtnHeight,
                    (Color){50, 50, 60, 255});
      DrawRectangleLines(menuBtnX, menuBtnY, menuBtnWidth, menuBtnHeight,
                         LIGHTGRAY);

      menuTextWidth = MeasureText("MENU", 28);
      DrawText("MENU", menuBtnX + (menuBtnWidth - menuTextWidth) / 2,
               menuBtnY + 13, 28, WHITE);

      if (IsKeyPressed(KEY_M)) {
        optionsOpen = 0;
        gameState = STATE_MENU;
      }
      if (checkMenuButtonPress(menuBtnX, menuBtnY, menuBtnWidth,
                               menuBtnHeight)) {
        optionsOpen = 0;
        gameState = STATE_MENU;
      }

    } break;

    case STATE_END_OF_ROUND: {
      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);
      drawEndOfRound();

      int btnWidth = 250;
      int btnHeight = 55;
      int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
      int gameAreaWidth = sidebarX;
      int boxWidth = 700;
      int boxX = (gameAreaWidth - boxWidth) / 2;
      int boxY = 50;
      int boxHeight = 600;

      int btnX = boxX + boxWidth / 2 - btnWidth / 2;
      int btnY = boxY + boxHeight - 65; // ✅ Coordenada corregida

      Vector2 mouse = GetMousePosition();
      Rectangle btnRect = {btnX, btnY, btnWidth, btnHeight};

      if (CheckCollisionPointRec(mouse, btnRect) &&
          IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (roundCompleted) {
          money += roundMoney;
          gameState = STATE_CATA_SELECT;
        } else {
          gameState = STATE_GAMEOVER;
        }
      }

      if (IsKeyPressed(KEY_C)) {
        if (roundCompleted) {
          money += roundMoney;
          gameState = STATE_CATA_SELECT;
        } else {
          gameState = STATE_GAMEOVER;
        }
      }

    } break;

    case STATE_OPTIONS: {
      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);
      DrawRectangle(0, 0, sidebarX, SCREEN_HEIGHT, (Color){0, 0, 0, 220});

      DrawText("OPTIONS", sidebarX / 2 - 120, SCREEN_HEIGHT / 2 - 100, 60,
               SKYBLUE);
      DrawText("More settings coming soon!", sidebarX / 2 - 140,
               SCREEN_HEIGHT / 2 - 20, 25, LIGHTGRAY);

      DrawRectangle(resumeBtnX, resumeBtnY, resumeBtnWidth, resumeBtnHeight,
                    (Color){50, 150, 50, 255});
      DrawRectangleLines(resumeBtnX, resumeBtnY, resumeBtnWidth,
                         resumeBtnHeight, LIGHTGRAY);
      int resumeTextWidth = MeasureText("RESUME", 28);
      DrawText("RESUME", resumeBtnX + (resumeBtnWidth - resumeTextWidth) / 2,
               resumeBtnY + 13, 28, WHITE);

      DrawRectangle(menuBtnX, menuBtnY + 70, menuBtnWidth, menuBtnHeight,
                    (Color){50, 50, 60, 255});
      DrawRectangleLines(menuBtnX, menuBtnY + 70, menuBtnWidth, menuBtnHeight,
                         LIGHTGRAY);

      menuTextWidth = MeasureText("MENU", 28);
      DrawText("MENU", menuBtnX + (menuBtnWidth - menuTextWidth) / 2,
               menuBtnY + 83, 28, WHITE);

      if (IsKeyPressed(KEY_M)) {
        optionsOpen = false;
        gameState = STATE_MENU;
      }
      if (checkResumeButtonPress(resumeBtnX, resumeBtnY, resumeBtnWidth,
                                 resumeBtnHeight)) {
        optionsOpen = false;
        gameState = STATE_PLAYING;
      }
      if (checkMenuButtonPress(menuBtnX, menuBtnY + 70, menuBtnWidth,
                               menuBtnHeight)) {
        optionsOpen = false;
        gameState = STATE_MENU;
      }

    } break;

    case STATE_WIN: {
      drawSidebar();
      drawBottomBar();
      drawBoard(offsetX, offsetY, currentTileSize);
      DrawRectangle(0, 0, sidebarX, SCREEN_HEIGHT, (Color){0, 0, 0, 200});

      DrawText("YOU WIN!", sidebarX / 2 - 150, SCREEN_HEIGHT / 2 - 40, 60,
               GREEN);
      DrawText(TextFormat("FINAL SCORE: %i", score), sidebarX / 2 - 120,
               SCREEN_HEIGHT / 2 + 20, 30, GOLD);
      DrawText("PRESS R TO PLAY AGAIN", sidebarX / 2 - 160,
               SCREEN_HEIGHT / 2 + 70, 25, WHITE);

      if (IsKeyPressed(KEY_R)) {
        initGame();
        gameState = STATE_PLAYING;
      }
      if (IsKeyPressed(KEY_M))
        gameState = STATE_MENU;

    } break;
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
