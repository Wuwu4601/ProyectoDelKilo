#include "../include/ui.h"
#include "../include/config.h"
#include "../include/board.h"
#include "../include/game.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

static float flameTimer = 0;
static bool showFlames = false;

void checkFlameEffect()
{
    if (score >= targetScore * 1.25f) {
        showFlames = true;
        flameTimer += GetFrameTime() * 2;
    } else {
        showFlames = false;
        flameTimer = 0;
    }
}

void drawFlame(int x, int y, int size, float timer)
{
    Color flameColors[] = {
        (Color){255, 100, 0, 255},
        (Color){255, 150, 0, 255},
        (Color){255, 200, 0, 255},
        (Color){255, 255, 100, 255}
    };
    
    int colorIdx = (int)(timer * 10) % 4;
    
    DrawCircle(x, y, size, flameColors[colorIdx]);
    DrawCircle(x, y - size/3, size * 0.7, flameColors[(colorIdx + 1) % 4]);
    DrawCircle(x, y - size/2, size * 0.4, flameColors[(colorIdx + 2) % 4]);
}

// ✅ Bomba simple (círculo negro)
void drawMine(int centerX, int centerY, int tileSize)
{
    int mineSize = tileSize / 4;  // ✅ Un poco más grande para que se vea bien
    
    // Círculo central negro
    DrawCircle(centerX, centerY, mineSize, BLACK);
}

// ✅ Sidebar para gameplay (completo)
void drawSidebar()
{
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    
    DrawRectangle(sidebarX, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, (Color){25, 25, 35, 255});
    DrawRectangle(sidebarX, 0, 2, SCREEN_HEIGHT, (Color){50, 50, 60, 255});
    
    int xOffset = sidebarX + 20;
    int yOffset = 30;
    int sectionSpacing = 25;
    int itemSpacing = 42;
    
    // CURRENT ROUND
    DrawText("CURRENT ROUND", xOffset, yOffset, 16, LIGHTGRAY);
    yOffset += 25;
    
    const char* roundName = getRoundTypeName(roundType);
    Color roundColor = getRoundTypeColor(roundType);
    
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 60, (Color){40, 40, 50, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 60, roundColor);
    DrawText(roundName, xOffset + 45, yOffset + 20, 36, roundColor);
    DrawCircle(xOffset + 15, yOffset + 30, 10, roundColor);
    yOffset += 70 + sectionSpacing;
    
    // TARGET SCORE
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 55, (Color){35, 25, 25, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 55, RED);
    DrawText("TARGET SCORE", xOffset + 5, yOffset + 8, 14, RED);
    DrawText(TextFormat("%i", targetScore), xOffset + 5, yOffset + 28, 28, RED);
    DrawText("Reward $$$", xOffset + 140, yOffset + 30, 16, GOLD);
    yOffset += 65 + sectionSpacing;
    
    // CURRENT SCORE
    DrawText("CURRENT SCORE", xOffset, yOffset, 16, LIGHTGRAY);
    yOffset += 25;
    
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 130, (Color){30, 30, 40, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 130, (Color){70, 70, 80, 255});
    
    int totalScore = baseScore * mult;
    
    DrawRectangle(xOffset + 10, yOffset + 10, SIDEBAR_WIDTH - 60, 50, (Color){60, 60, 70, 255});
    DrawRectangleLines(xOffset + 10, yOffset + 10, SIDEBAR_WIDTH - 60, 50, GOLD);
    
    int fontSize = 32;
    char scoreText[20];
    sprintf(scoreText, "%i", totalScore);
    int textWidth = MeasureText(scoreText, fontSize);
    int boxWidth = SIDEBAR_WIDTH - 60;
    int textX = xOffset + 10 + (boxWidth - textWidth) / 2;
    int textY = yOffset + 10 + (50 - fontSize) / 2;
    
    if (showFlames) {
        for (int i = 0; i < 5; i++) {
            int flameX = textX + textWidth/2 + (int)(sin(flameTimer + i) * 10);
            int flameY = textY + fontSize/2 + (int)(cos(flameTimer + i * 0.5) * 5);
            drawFlame(flameX, flameY, 6 + (i % 3), flameTimer);
        }
        DrawText(scoreText, textX, textY, fontSize, (Color){255, 200, 50, 255});
    } else {
        DrawText(scoreText, textX, textY, fontSize, GOLD);
    }
    
    DrawText("x", xOffset + 105, yOffset + 75, 28, RED);
    
    DrawRectangle(xOffset + 15, yOffset + 70, 75, 45, (Color){50, 100, 200, 255});
    DrawRectangleLines(xOffset + 15, yOffset + 70, 75, 45, WHITE);
    
    char baseText[20];
    sprintf(baseText, "%i", baseScore);
    int scoreWidth = MeasureText(baseText, 28);
    int baseX = xOffset + 15 + (75 - scoreWidth) / 2;
    int baseY = yOffset + 70 + (45 - 28) / 2;
    DrawText(baseText, baseX, baseY, 28, WHITE);
    
    DrawRectangle(xOffset + 130, yOffset + 70, 75, 45, (Color){200, 50, 50, 255});
    DrawRectangleLines(xOffset + 130, yOffset + 70, 75, 45, WHITE);
    char multText[20];
    sprintf(multText, "%i", mult);
    int multWidth = MeasureText(multText, 28);
    int multX = xOffset + 130 + (75 - multWidth) / 2;
    int multY = yOffset + 70 + (45 - 28) / 2;
    DrawText(multText, multX, multY, 28, WHITE);
    
    yOffset += 140 + sectionSpacing;
    
    // RUN INFO
    DrawText("RUN INFO", xOffset, yOffset, 16, LIGHTGRAY);
    yOffset += 25;
    
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 240, (Color){35, 35, 45, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 240, (Color){60, 60, 70, 255});
    
    int infoY = yOffset + 20;
    
    DrawRectangle(xOffset + 5, infoY, SIDEBAR_WIDTH - 50, 38, (Color){40, 30, 30, 255});
    DrawRectangleLines(xOffset + 5, infoY, SIDEBAR_WIDTH - 50, 38, RED);
    DrawText("LIVES", xOffset + 15, infoY + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%i", currentLives), xOffset + 120, infoY + 8, 26, RED);
    
    DrawRectangle(xOffset + 5, infoY + itemSpacing, SIDEBAR_WIDTH - 50, 38, (Color){45, 40, 30, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing, SIDEBAR_WIDTH - 50, 38, GOLD);
    DrawText("MONEY", xOffset + 15, infoY + itemSpacing + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("$%i", money), xOffset + 120, infoY + itemSpacing + 8, 26, GOLD);
    
    DrawRectangle(xOffset + 5, infoY + itemSpacing*2, SIDEBAR_WIDTH - 50, 38, (Color){40, 35, 25, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing*2, SIDEBAR_WIDTH - 50, 38, ORANGE);
    DrawText("CALICATA", xOffset + 15, infoY + itemSpacing*2 + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%i/10", calicataLevel), xOffset + 120, infoY + itemSpacing*2 + 8, 26, ORANGE);
    
    DrawRectangle(xOffset + 5, infoY + itemSpacing*3, SIDEBAR_WIDTH - 50, 38, (Color){30, 35, 45, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing*3, SIDEBAR_WIDTH - 50, 38, SKYBLUE);
    DrawText("ROUND", xOffset + 15, infoY + itemSpacing*3 + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%i", roundNumber), xOffset + 120, infoY + itemSpacing*3 + 8, 26, SKYBLUE);
    
    DrawRectangle(xOffset + 5, infoY + itemSpacing*4, SIDEBAR_WIDTH - 50, 38, (Color){30, 40, 35, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing*4, SIDEBAR_WIDTH - 50, 38, GREEN);
    DrawText("REVEALED", xOffset + 15, infoY + itemSpacing*4 + 10, 16, LIGHTGRAY);
    int totalSafeTiles = (currentGridW * currentGridH) - currentMines;
    DrawText(TextFormat("%i/%i", totalTilesRevealed, totalSafeTiles), xOffset + 105, infoY + itemSpacing*4 + 8, 22, GREEN);
    
    yOffset += 250 + sectionSpacing;
    
    // OPTIONS BUTTON
    int optionsWidth = 140;
    int optionsHeight = 50;
    int optionsX = xOffset + (SIDEBAR_WIDTH - 40 - optionsWidth) / 2;
    int optionsY = SCREEN_HEIGHT - 65;
    DrawRectangle(optionsX, optionsY, optionsWidth, optionsHeight, (Color){50, 50, 60, 255});
    DrawRectangleLines(optionsX, optionsY, optionsWidth, optionsHeight, (Color){100, 100, 120, 255});
    
    int optionsTextWidth = MeasureText("OPTIONS", 26);
    int optionsTextX = optionsX + (optionsWidth - optionsTextWidth) / 2;
    int optionsTextY = optionsY + (optionsHeight - 26) / 2;
    DrawText("OPTIONS", optionsTextX, optionsTextY, 26, LIGHTGRAY);
}

// ✅ Sidebar para Cata Selector (con correcciones estéticas)
void drawCataSelectorSidebar()
{
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    
    DrawRectangle(sidebarX, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, (Color){25, 25, 35, 255});
    DrawRectangle(sidebarX, 0, 2, SCREEN_HEIGHT, (Color){50, 50, 60, 255});
    
    int xOffset = sidebarX + 20;
    int yOffset = 30;
    int sectionSpacing = 25;
    int itemSpacing = 42;
    
    // ✅ CURRENT ROUND → "CHOOSE YOUR NEXT CATA" (texto más pequeño)
    DrawText("CURRENT ROUND", xOffset, yOffset, 16, LIGHTGRAY);
    yOffset += 25;
    
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 60, (Color){40, 40, 50, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 60, (Color){100, 200, 255, 255});
    DrawText("CHOOSE YOUR NEXT CATA", xOffset + 10, yOffset + 22, 16, WHITE);  // ✅ Texto más pequeño (16 en vez de 20)
    yOffset += 70 + sectionSpacing;
    
    // ✅ TARGET SCORE - Cuadro vacío (tono rojizo opaco)
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 55, (Color){40, 25, 25, 255});  // ✅ Rojo opaco
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 55, (Color){100, 50, 50, 255});  // ✅ Borde rojizo
    DrawText("TARGET SCORE", xOffset + 5, yOffset + 8, 14, (Color){120, 50, 50, 255});  // ✅ Texto rojizo opaco
    yOffset += 65 + sectionSpacing;
    
    // ✅ CURRENT SCORE - Cuadro vacío
    DrawText("CURRENT SCORE", xOffset, yOffset, 16, (Color){100, 100, 100, 255});
    yOffset += 25;
    
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 130, (Color){30, 30, 40, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 130, (Color){100, 100, 100, 255});
    yOffset += 140 + sectionSpacing;
    
    // ✅ RUN INFO (más alto para que REVEALED quepa)
    DrawText("RUN INFO", xOffset, yOffset, 16, LIGHTGRAY);
    yOffset += 25;
    
    // ✅ Caja más alta (240 en vez de 200)
    DrawRectangle(xOffset, yOffset, SIDEBAR_WIDTH - 40, 240, (Color){35, 35, 45, 255});
    DrawRectangleLines(xOffset, yOffset, SIDEBAR_WIDTH - 40, 240, (Color){60, 60, 70, 255});
    
    int infoY = yOffset + 20;
    
    // LIVES
    DrawRectangle(xOffset + 5, infoY, SIDEBAR_WIDTH - 50, 38, (Color){40, 30, 30, 255});
    DrawRectangleLines(xOffset + 5, infoY, SIDEBAR_WIDTH - 50, 38, RED);
    DrawText("LIVES", xOffset + 15, infoY + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%i", currentLives), xOffset + 120, infoY + 8, 26, RED);
    
    // MONEY
    DrawRectangle(xOffset + 5, infoY + itemSpacing, SIDEBAR_WIDTH - 50, 38, (Color){45, 40, 30, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing, SIDEBAR_WIDTH - 50, 38, GOLD);
    DrawText("MONEY", xOffset + 15, infoY + itemSpacing + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("$%i", money), xOffset + 120, infoY + itemSpacing + 8, 26, GOLD);
    
    // CALICATA
    DrawRectangle(xOffset + 5, infoY + itemSpacing*2, SIDEBAR_WIDTH - 50, 38, (Color){40, 35, 25, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing*2, SIDEBAR_WIDTH - 50, 38, ORANGE);
    DrawText("CALICATA", xOffset + 15, infoY + itemSpacing*2 + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%i/10", calicataLevel), xOffset + 120, infoY + itemSpacing*2 + 8, 26, ORANGE);
    
    // ROUND
    DrawRectangle(xOffset + 5, infoY + itemSpacing*3, SIDEBAR_WIDTH - 50, 38, (Color){30, 35, 45, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing*3, SIDEBAR_WIDTH - 50, 38, SKYBLUE);
    DrawText("ROUND", xOffset + 15, infoY + itemSpacing*3 + 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%i", roundNumber), xOffset + 120, infoY + itemSpacing*3 + 8, 26, SKYBLUE);
    
    // ✅ REVEALED - Cuadro vacío (dentro de RUN INFO)
    DrawRectangle(xOffset + 5, infoY + itemSpacing*4, SIDEBAR_WIDTH - 50, 38, (Color){30, 40, 35, 255});
    DrawRectangleLines(xOffset + 5, infoY + itemSpacing*4, SIDEBAR_WIDTH - 50, 38, (Color){60, 100, 60, 255});  // ✅ Borde verde opaco
    DrawText("REVEALED", xOffset + 15, infoY + itemSpacing*4 + 10, 16, (Color){80, 120, 80, 255});  // ✅ Texto verde opaco
    // ✅ Sin números
    
    // ✅ No aumentar yOffset aquí - REVEALED es el último item
    
    yOffset += 250 + sectionSpacing;
    
    // OPTIONS BUTTON
    int optionsWidth = 140;
    int optionsHeight = 50;
    int optionsX = xOffset + (SIDEBAR_WIDTH - 40 - optionsWidth) / 2;
    int optionsY = SCREEN_HEIGHT - 65;
    DrawRectangle(optionsX, optionsY, optionsWidth, optionsHeight, (Color){50, 50, 60, 255});
    DrawRectangleLines(optionsX, optionsY, optionsWidth, optionsHeight, (Color){100, 100, 120, 255});
    
    int optionsTextWidth = MeasureText("OPTIONS", 26);
    int optionsTextX = optionsX + (optionsWidth - optionsTextWidth) / 2;
    int optionsTextY = optionsY + (optionsHeight - 26) / 2;
    DrawText("OPTIONS", optionsTextX, optionsTextY, 26, LIGHTGRAY);
}

void drawBottomBar()
{
    int barHeight = 70;
    int barY = SCREEN_HEIGHT - barHeight;
    
    int currentTileSize = (roundType == ROUND_CATITO) ? TILE_SIZE_CATITO : TILE_SIZE;
    int boardPixelWidth = currentGridW * currentTileSize;
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int boardStartX = (sidebarX - boardPixelWidth) / 2;
    
    DrawRectangle(boardStartX, barY, boardPixelWidth, barHeight, (Color){25, 25, 35, 255});
    DrawRectangle(boardStartX, barY, boardPixelWidth, 2, (Color){50, 50, 60, 255});
    
    int sectionWidth = boardPixelWidth / 2 - 10;
    
    // ✅ MINES - ROJO (usando minesLeft directamente)
    DrawRectangle(boardStartX + 10, barY + 10, sectionWidth - 20, 50, (Color){40, 25, 25, 255});
    DrawRectangleLines(boardStartX + 10, barY + 10, sectionWidth - 20, 50, RED);
    DrawText("MINES", boardStartX + 40, barY + 20, 20, LIGHTGRAY);
    
    // ✅ Usar minesLeft directamente (variable global de game.c)
    char minesText[20];
    sprintf(minesText, "%i", minesLeft);  // ✅ minesLeft, NO actualMines
    int minesWidth = MeasureText(minesText, 36);
    int minesX = boardStartX + 50 + (sectionWidth/2 - minesWidth/2);
    DrawText(minesText, minesX, barY + 20, 36, RED);
    
    // ✅ TIME
    DrawRectangle(boardStartX + sectionWidth + 10, barY + 10, sectionWidth - 20, 50, (Color){30, 40, 35, 255});
    DrawRectangleLines(boardStartX + sectionWidth + 10, barY + 10, sectionWidth - 20, 50, GREEN);
    DrawText("TIME", boardStartX + sectionWidth + 40, barY + 20, 20, LIGHTGRAY);
    
    char timeText[20];
    sprintf(timeText, "%.1f", gameTime);
    int timeWidth = MeasureText(timeText, 36);
    int timeX = boardStartX + sectionWidth + 50 + (sectionWidth/2 - timeWidth/2);
    DrawText(timeText, timeX, barY + 20, 36, GREEN);
}

void drawBoard(int offsetX, int offsetY, int tileSize)
{
    for (int y = 0; y < currentGridH; y++) {
        for (int x = 0; x < currentGridW; x++) {
            int sx = offsetX + x * tileSize;
            int sy = offsetY + y * tileSize;
            
            Color covered = (Color){70, 70, 70, 255};
            Color revealedColor = (Color){200, 200, 200, 255};
            
            if (revealed[y][x]) {
                DrawRectangle(sx, sy, tileSize, tileSize, revealedColor);
                
                if (board[y][x] > 0) {
                    Color numColor = WHITE;
                    switch (board[y][x]) {
                        case 1: numColor = BLUE; break;
                        case 2: numColor = (Color){0, 150, 0, 255}; break;
                        case 3: numColor = RED; break;
                        case 4: numColor = (Color){0, 0, 130, 255}; break;
                        case 5: numColor = (Color){120, 0, 0, 255}; break;
                        case 6: numColor = (Color){0, 120, 120, 255}; break;
                        case 7: numColor = BLACK; break;
                        default: numColor = DARKGRAY; break;
                    }
                    DrawText(TextFormat("%i", board[y][x]), sx + tileSize/2 - 10, sy + tileSize/2 - 12, 24, numColor);
                }
                
                if (board[y][x] == -1) {
                    // ✅ Bomba con pinchos
                    drawMine(sx + tileSize/2, sy + tileSize/2, tileSize);
                }
            } else {
                DrawRectangle(sx, sy, tileSize, tileSize, covered);
                
                if (flagged[y][x]) {
                    Vector2 points[3] = {
                        {(float)sx + tileSize/3, (float)sy + tileSize/3},
                        {(float)sx + tileSize/3, (float)sy + tileSize*2/3},
                        {(float)sx + tileSize*2/3, (float)sy + tileSize/2}
                    };
                    DrawTriangle(points[0], points[1], points[2], RED);
                }
            }
            
            DrawRectangleLines(sx, sy, tileSize, tileSize, BLACK);
        }
    }
}

// ✅ CATA SELECTOR - Centrado perfectamente + Sidebar integrado
void drawCataSelector()
{
    // ✅ NO dibujar fondo negro - el sidebar ya está integrado
    drawCataSelectorSidebar();
    
    // ✅ Centrado perfecto en el área disponible
    int cardWidth = 220;
    int cardHeight = 450;
    int cardSpacing = 40;
    int startY = 120;
    
    // ✅ Calcular centro del área negra (no del SCREEN_WIDTH)
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int gameAreaWidth = sidebarX;
    int totalCardsWidth = cardWidth * 3 + cardSpacing * 2;
    int startX = (gameAreaWidth - totalCardsWidth) / 2;
    
    // ========== SONDEO ==========
    int card1X = startX;
    DrawRectangle(card1X, startY, cardWidth, cardHeight, (Color){30, 40, 60, 255});
    DrawRectangleLines(card1X, startY, cardWidth, cardHeight, (Color){100, 200, 255, 255});
    
    // ✅ Botón Select/Completed
    if (sondeoBeaten) {
        DrawRectangle(card1X + 20, startY + 20, cardWidth - 40, 45, (Color){80, 80, 80, 255});
        DrawText("Completed", card1X + 55, startY + 28, 24, LIGHTGRAY);
    } else {
        DrawRectangle(card1X + 20, startY + 20, cardWidth - 40, 45, (Color){255, 150, 50, 255});
        DrawText("Select", card1X + 65, startY + 28, 26, BLACK);
    }
    
    DrawCircle(card1X + cardWidth/2, startY + 150, 60, (Color){50, 100, 200, 255});
    
    const char* sondeoText = "SONDEO";
    int sondeoWidth = MeasureText(sondeoText, 22);
    DrawText(sondeoText, card1X + cardWidth/2 - sondeoWidth/2, startY + 140, 22, WHITE);
    
    char sondeoScoreText[20];
    sprintf(sondeoScoreText, "%i", SCORE_SONDEO + (calicataLevel - 1) * 30);
    int sondeoScoreWidth = MeasureText(sondeoScoreText, 36);
    DrawText(sondeoScoreText, card1X + cardWidth/2 - sondeoScoreWidth/2, startY + 230, 36, (Color){255, 100, 100, 255});
    
    DrawText("Reward: $$$", card1X + cardWidth/2 - 50, startY + 280, 22, GOLD);
    
    // ✅ Skip button (solo si NO completado)
    if (!sondeoBeaten) {
        DrawRectangle(card1X + 20, startY + 360, cardWidth - 40, 55, (Color){200, 50, 50, 255});
        DrawText("Skip", card1X + 85, startY + 375, 24, WHITE);
    }
    
// ========== CATA ==========
int card2X = startX + cardWidth + cardSpacing;
DrawRectangle(card2X, startY, cardWidth, cardHeight, (Color){40, 35, 25, 255});
DrawRectangleLines(card2X, startY, cardWidth, cardHeight, (Color){255, 200, 100, 255});

// ✅ Botón Select/Completed/Upcoming
if (sondeoBeaten && !cataBeaten) {
    DrawRectangle(card2X + 20, startY + 20, cardWidth - 40, 45, (Color){255, 150, 50, 255});
    DrawText("Select", card2X + 65, startY + 28, 26, BLACK);
} else {
    DrawRectangle(card2X + 20, startY + 20, cardWidth - 40, 45, (Color){80, 80, 80, 255});
    DrawText(cataBeaten ? "Completed" : "Upcoming", card2X + 55, startY + 28, 24, LIGHTGRAY);
}

// ✅ Círculo AMARILLO FIJO (eliminar el Color cataCircleColor)
DrawCircle(card2X + cardWidth/2, startY + 150, 60, (Color){200, 150, 50, 255});

const char* cataText = "CATA";
int cataWidth = MeasureText(cataText, 22);
DrawText(cataText, card2X + cardWidth/2 - cataWidth/2, startY + 140, 22, WHITE);

char cataScoreText[20];
sprintf(cataScoreText, "%i", SCORE_CATA + (calicataLevel - 1) * 50);
int cataScoreWidth = MeasureText(cataScoreText, 36);
DrawText(cataScoreText, card2X + cardWidth/2 - cataScoreWidth/2, startY + 230, 36, (Color){255, 100, 100, 255});

DrawText("Reward: $$$$+", card2X + cardWidth/2 - 55, startY + 280, 22, (Color){200, 200, 100, 255});

// ✅ Skip Cata (visible si Sondeo está completado/skippeado)
if (sondeoBeaten && !cataBeaten) {
    DrawRectangle(card2X + 20, startY + 360, cardWidth - 40, 55, (Color){150, 50, 200, 255});
    DrawText("Skip", card2X + 85, startY + 375, 24, WHITE);
}
    
// ========== CALICATA ==========
int card3X = startX + (cardWidth + cardSpacing) * 2;
DrawRectangle(card3X, startY, cardWidth, cardHeight, (Color){45, 25, 25, 255});
DrawRectangleLines(card3X, startY, cardWidth, cardHeight, (Color){255, 100, 100, 255});

// ✅ Botón Select/Completed/Upcoming
if (sondeoBeaten && cataBeaten && !calicataBeaten) {
    DrawRectangle(card3X + 20, startY + 20, cardWidth - 40, 45, (Color){255, 150, 50, 255});
    DrawText("Select", card3X + 65, startY + 28, 26, BLACK);
} else {
    DrawRectangle(card3X + 20, startY + 20, cardWidth - 40, 45, (Color){80, 80, 80, 255});
    DrawText(calicataBeaten ? "Completed" : "Upcoming", card3X + 55, startY + 28, 24, LIGHTGRAY);
}

// ✅ Círculo ROJO FIJO (eliminar Color calicataCircleColor)
DrawCircle(card3X + cardWidth/2, startY + 150, 60, (Color){200, 50, 50, 255});

const char* calicataText = "CALICATA";
int calicataWidth = MeasureText(calicataText, 22);
DrawText(calicataText, card3X + cardWidth/2 - calicataWidth/2, startY + 140, 22, WHITE);

char calicataScoreText[20];
sprintf(calicataScoreText, "%i", SCORE_CALICATA + (calicataLevel - 1) * 100);
int calicataScoreWidth = MeasureText(calicataScoreText, 36);
DrawText(calicataScoreText, card3X + cardWidth/2 - calicataScoreWidth/2, startY + 230, 36, (Color){255, 100, 100, 255});

DrawText("Reward: $$$$$+", card3X + cardWidth/2 - 60, startY + 280, 22, (Color){200, 150, 150, 255});

// ✅ CALICATA NO TIENE BOTÓN SKIP
}

// ✅ END OF ROUND - Estilo único de Scopae Minarum (CORREGIDO)
void drawEndOfRound()
{
    // ✅ Obtener color de la ronda actual
    Color roundColor = getRoundTypeColor(roundType);
    
    // ✅ Fondo semi-transparente (oscurecer todo el juego)
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
    
    // ✅ Caja principal (TOP BAR - centrada en el área negra del juego)
    int boxWidth = 700;  // ✅ Más ancha
    int boxHeight = 600;  // ✅ Más alta
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int gameAreaWidth = sidebarX;
    int boxX = (gameAreaWidth - boxWidth) / 2;  // ✅ Centrado en el área negra
    int boxY = 50;  // ✅ TOP BAR (50px desde arriba)
    
    // Fondo con gradiente (simulado con rectángulos)
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, (Color){20, 25, 35, 255});
    
    // Borde del color de la ronda
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, roundColor);
    DrawRectangleLines(boxX + 2, boxY + 2, boxWidth - 4, boxHeight - 4, roundColor);
    
    // ✅ Título con glow del color de la ronda
    DrawText("END OF ROUND", boxX + boxWidth/2 - 140, boxY + 25, 40, roundColor);
    
    int contentX = boxX + 40;
    int yOffset = boxY + 85;
    int lineHeight = 35;
    
    // ═══════════════════════════════════════════
    // SECCIÓN 1: SCORE BREAKDOWN
    // ═══════════════════════════════════════════
    
    DrawText("SCORE BREAKDOWN", contentX, yOffset, 20, LIGHTGRAY);
    yOffset += 30;
    
    // Caja de score
    DrawRectangle(contentX, yOffset, boxWidth - 80, 150, (Color){30, 35, 45, 255});  // ✅ Más alta
    DrawRectangleLines(contentX, yOffset, boxWidth - 80, 150, (Color){80, 80, 100, 255});
    
    int scoreX = contentX + 20;
    int scoreY = yOffset + 15;
    
    // Base Score
    DrawText("Base Score:", scoreX, scoreY, 22, LIGHTGRAY);
    char baseText[20];
    sprintf(baseText, "%i", score);
    int baseWidth = MeasureText(baseText, 22);
    DrawText(baseText, scoreX + boxWidth - 120 - baseWidth, scoreY, 22, GOLD);
    scoreY += 35;
    
    // ✅ Par Time Bonus - Mostrar cálculo completo
    DrawText("Par Time Bonus:", scoreX, scoreY, 22, LIGHTGRAY);
    char parTimeCalc[50];
    sprintf(parTimeCalc, "(%is - %.1fs) × %.1f = %i", currentParTime, gameTime, (float)parTimeMultiplier, timeBonus);
    int parTimeWidth = MeasureText(parTimeCalc, 22);
    DrawText(parTimeCalc, scoreX + boxWidth - 120 - parTimeWidth, scoreY, 22, GREEN);
    scoreY += 35;
    
    // Línea separadora
    DrawLine(scoreX, scoreY + 5, scoreX + boxWidth - 100, scoreY + 5, (Color){80, 80, 100, 255});
    scoreY += 15;
    
    // Total Score
    DrawText("TOTAL SCORE:", scoreX, scoreY, 26, WHITE);
    char totalText[20];
    sprintf(totalText, "%i", totalScore);
    int totalWidth = MeasureText(totalText, 26);
    DrawText(totalText, scoreX + boxWidth - 120 - totalWidth, scoreY, 26, GOLD);
    
    yOffset += 165;
    
    // ═══════════════════════════════════════════
    // SECCIÓN 2: TARGET & RESULT
    // ═══════════════════════════════════════════
    
    DrawText("TARGET INFO", contentX, yOffset, 20, LIGHTGRAY);
    yOffset += 30;
    
    // Caja de target
    DrawRectangle(contentX, yOffset, boxWidth - 80, 100, (Color){30, 35, 45, 255});
    DrawRectangleLines(contentX, yOffset, boxWidth - 80, 100, (Color){80, 80, 100, 255});
    
    int targetX = contentX + 20;
    int targetY = yOffset + 15;
    
    DrawText("Target Score:", targetX, targetY, 22, LIGHTGRAY);
    char targetText[20];
    sprintf(targetText, "%i", targetScore);
    int targetWidth = MeasureText(targetText, 22);
    DrawText(targetText, targetX + boxWidth - 120 - targetWidth, targetY, 22, RED);
    targetY += 35;
    
    DrawText("RESULT:", targetX, targetY, 22, LIGHTGRAY);
    const char* resultText = roundCompleted ? "COMPLETED" : "FAILED";
    Color resultColor = roundCompleted ? GREEN : RED;
    int resultWidth = MeasureText(resultText, 22);
    DrawText(resultText, targetX + boxWidth - 120 - resultWidth, targetY, 22, resultColor);
    
    yOffset += 115;
    
    // ═══════════════════════════════════════════
    // SECCIÓN 3: MENSAJE DE ERROR (si FAILED)
    // ═══════════════════════════════════════════
    
    if (!roundCompleted) {
        float alpha = (sin(GetTime() * 5) + 1) / 2 * 127 + 128;
        Color warningColor = (Color){200, 50, 50, (unsigned char)alpha};
        
        DrawRectangle(contentX, yOffset, boxWidth - 80, 50, (Color){40, 20, 20, 255});
        DrawRectangleLines(contentX, yOffset, boxWidth - 80, 50, warningColor);
        
        const char* warningText = "⚠ TOO SLOW - TARGET NOT REACHED ⚠";
        int warningWidth = MeasureText(warningText, 20);
        DrawText(warningText, contentX + (boxWidth - 80 - warningWidth) / 2, yOffset + 15, 20, warningColor);
        
        yOffset += 60;
    }
    
    // ═══════════════════════════════════════════
    // SECCIÓN 4: DINERO
    // ═══════════════════════════════════════════
    
    DrawText("MONEY", contentX, yOffset, 20, LIGHTGRAY);
    yOffset += 30;
    
    // Caja de dinero
    DrawRectangle(contentX, yOffset, boxWidth - 80, 80, (Color){30, 35, 45, 255});
    DrawRectangleLines(contentX, yOffset, boxWidth - 80, 80, (Color){80, 80, 100, 255});
    
    int moneyX = contentX + 20;
    int moneyY = yOffset + 15;
    
    DrawText("Money Earned:", moneyX, moneyY, 22, LIGHTGRAY);
    char earnedText[20];
    sprintf(earnedText, "$%i", roundCompleted ? roundMoney : 0);
    int earnedWidth = MeasureText(earnedText, 22);
    DrawText(earnedText, moneyX + boxWidth - 120 - earnedWidth, moneyY, 22, GOLD);
    moneyY += 30;
    
    DrawText("Total Money:", moneyX, moneyY, 22, LIGHTGRAY);
    char totalMoneyText[20];
    sprintf(totalMoneyText, "$%i", money + (roundCompleted ? roundMoney : 0));
    int totalMoneyWidth = MeasureText(totalMoneyText, 22);
    DrawText(totalMoneyText, moneyX + boxWidth - 120 - totalMoneyWidth, moneyY, 22, GOLD);
    
    yOffset += 95;
    
    // ═══════════════════════════════════════════
    // BOTÓN DE ACCIÓN (DEBAJO del dinero, no encima)
    // ═══════════════════════════════════════════
    
    int btnWidth = 250;
    int btnHeight = 55;
    int btnX = boxX + boxWidth/2 - btnWidth/2;
    int btnY = boxY + boxHeight - 65;  // ✅ Más abajo para no cubrir el dinero
    
    Color btnColor = roundCompleted ? (Color){50, 180, 50, 255} : (Color){180, 50, 50, 255};
    const char* btnText = roundCompleted ? "CONTINUE" : "TRY AGAIN";
    
    DrawRectangle(btnX, btnY, btnWidth, btnHeight, btnColor);
    DrawRectangleLines(btnX, btnY, btnWidth, btnHeight, LIGHTGRAY);
    
    int btnTextWidth = MeasureText(btnText, 28);
    DrawText(btnText, btnX + (btnWidth - btnTextWidth) / 2, btnY + 14, 28, WHITE);
}

// ✅ Check Select button
bool checkCataSelectPress()
{
    Vector2 mouse = GetMousePosition();
    
    int cardWidth = 220;
    int cardSpacing = 40;
    int startY = 120;
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int gameAreaWidth = sidebarX;
    int totalCardsWidth = cardWidth * 3 + cardSpacing * 2;
    int startX = (gameAreaWidth - totalCardsWidth) / 2;
    
    // Determinar qué carta está disponible para Select
    if (!sondeoBeaten) {
        Rectangle selectRect = {startX + 20, startY + 20, cardWidth - 40, 45};
        if (CheckCollisionPointRec(mouse, selectRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    } else if (!cataBeaten) {
        int card2X = startX + cardWidth + cardSpacing;
        Rectangle selectRect = {card2X + 20, startY + 20, cardWidth - 40, 45};
        if (CheckCollisionPointRec(mouse, selectRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    } else if (!calicataBeaten) {
        int card3X = startX + (cardWidth + cardSpacing) * 2;
        Rectangle selectRect = {card3X + 20, startY + 20, cardWidth - 40, 45};
        if (CheckCollisionPointRec(mouse, selectRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    
    return false;
}

// ✅ Check Skip Sondeo button
bool checkSkipSondeoPress()
{
    Vector2 mouse = GetMousePosition();
    
    int cardWidth = 220;
    int cardSpacing = 40;
    int startY = 120;
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int gameAreaWidth = sidebarX;
    int totalCardsWidth = cardWidth * 3 + cardSpacing * 2;
    int startX = (gameAreaWidth - totalCardsWidth) / 2;
    
    if (sondeoBeaten) return false;
    
    Rectangle skipRect = {startX + 20, startY + 360, cardWidth - 40, 55};
    
    if (CheckCollisionPointRec(mouse, skipRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }
    
    return false;
}

// ✅ Check Skip Cata button
bool checkSkipCataPress()
{
    Vector2 mouse = GetMousePosition();
    
    int cardWidth = 220;
    int cardSpacing = 40;
    int startY = 120;
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int gameAreaWidth = sidebarX;
    int totalCardsWidth = cardWidth * 3 + cardSpacing * 2;
    int startX = (gameAreaWidth - totalCardsWidth) / 2;
    
    int card2X = startX + cardWidth + cardSpacing;
    
    if (!sondeoBeaten || cataBeaten) return false;
    
    Rectangle skipRect = {card2X + 20, startY + 360, cardWidth - 40, 55};
    
    if (CheckCollisionPointRec(mouse, skipRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }
    
    return false;
}

bool checkOptionsButtonPress()
{
    Vector2 mouse = GetMousePosition();
    int sidebarX = SCREEN_WIDTH - SIDEBAR_WIDTH - SIDEBAR_MARGIN;
    int optionsWidth = 140;
    int optionsHeight = 50;
    int optionsX = sidebarX + 20 + (SIDEBAR_WIDTH - 40 - optionsWidth) / 2;
    int optionsY = SCREEN_HEIGHT - 65;
    Rectangle optionsRect = {optionsX, optionsY, optionsWidth, optionsHeight};
    
    if (CheckCollisionPointRec(mouse, optionsRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    return false;
}

bool checkMenuButtonPress(int btnX, int btnY, int btnWidth, int btnHeight)
{
    Vector2 mouse = GetMousePosition();
    Rectangle menuRect = {(float)btnX, (float)btnY, (float)btnWidth, (float)btnHeight};
    
    if (CheckCollisionPointRec(mouse, menuRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    return false;
}

bool checkResumeButtonPress(int btnX, int btnY, int btnWidth, int btnHeight)
{
    Vector2 mouse = GetMousePosition();
    Rectangle resumeRect = {(float)btnX, (float)btnY, (float)btnWidth, (float)btnHeight};
    
    if (CheckCollisionPointRec(mouse, resumeRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    return false;
}

void updateUI()
{
    checkFlameEffect();
}