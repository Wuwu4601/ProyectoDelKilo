#include "../include/board.h"
#include "../include/config.h"
#include "../include/game.h"
#include "../include/save.h"
#include <stdlib.h>
#include <time.h>

int board[GRID_H_MAX][GRID_W_MAX];
bool revealed[GRID_H_MAX][GRID_W_MAX];
bool flagged[GRID_H_MAX][GRID_W_MAX];
bool minesPlaced = false;

// ✅ Limpiar TODO el tablero (no solo hasta currentGridW/H)
void clearBoard()
{
    for (int y = 0; y < GRID_H_MAX; y++) {
        for (int x = 0; x < GRID_W_MAX; x++) {
            board[y][x] = 0;
            revealed[y][x] = false;
            flagged[y][x] = false;
        }
    }
    minesPlaced = false;  // ✅ Resetear esto también
}

void placeMines()
{
    if (minesPlaced) return;
    
    srand(time(NULL) + currentGridW + currentGridH);
    
    int placed = 0;
    while (placed < currentMines) {
        int x = rand() % currentGridW;
        int y = rand() % currentGridH;
        
        if (board[y][x] != -1) {
            board[y][x] = -1;
            placed++;
        }
    }
    
    generateNumbers();
    minesPlaced = true;
}

// ✅ Safe first click - Asegurar que el primer click nunca tenga mina
void placeMinesAfterClick(int safeX, int safeY)
{
    if (minesPlaced) return;
    
    // ✅ Usar semilla diferente para cada ronda
    srand(time(NULL) + safeX + safeY + roundNumber);
    
    int placed = 0;
    while (placed < currentMines) {
        int x = rand() % currentGridW;
        int y = rand() % currentGridH;
        
        // ✅ No poner mina en el primer click ni sus vecinos (área 3x3)
        int dx = x - safeX;
        int dy = y - safeY;
        if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
            continue;
        }
        
        if (board[y][x] != -1) {
            board[y][x] = -1;
            placed++;
        }
    }
    
    generateNumbers();
    minesPlaced = true;
}

void generateNumbers()
{
    for (int y = 0; y < currentGridH; y++) {
        for (int x = 0; x < currentGridW; x++) {
            if (board[y][x] == -1) continue;
            
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    
                    if (nx >= 0 && nx < currentGridW && ny >= 0 && ny < currentGridH) {
                        if (board[ny][nx] == -1) {
                            count++;
                        }
                    }
                }
            }
            board[y][x] = count;
        }
    }
}

void revealTile(int x, int y)
{
    if (x < 0 || y < 0 || x >= currentGridW || y >= currentGridH) return;
    if (revealed[y][x] || flagged[y][x]) return;
    
    // ✅ Safe first click
    if (!minesPlaced) {
        placeMinesAfterClick(x, y);
    }
    
    revealed[y][x] = true;
    totalTilesRevealed++;
    save.totalTilesRevealed = totalTilesRevealed;
    
    if (board[y][x] > 0) {
        int points = board[y][x] * 2;
        addScore(points);
    } else if (board[y][x] == 0) {
        addScore(1);
    } else if (board[y][x] == -1) {
        totalMinesFound++;
        save.totalMinesFound = totalMinesFound;
        loseLife();
        return;
    }
    
    if (board[y][x] == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx != 0 || dy != 0) {
                    revealTile(x + dx, y + dy);
                }
            }
        }
    }
}

void toggleFlag(int x, int y)
{
    if (x < 0 || y < 0 || x >= currentGridW || y >= currentGridH) return;
    if (revealed[y][x]) return;
    
    flagged[y][x] = !flagged[y][x];
    
    // ✅ Cambiar minesLeft SIEMPRE (sin verificar si hay mina)
    if (flagged[y][x]) {
        minesLeft--;  // ✅ Bandera puesta - resta 1
    } else {
        minesLeft++;  // ✅ Bandera quitada - suma 1
    }
}
int countRevealed()
{
    int count = 0;
    for (int y = 0; y < currentGridH; y++) {
        for (int x = 0; x < currentGridW; x++) {
            if (revealed[y][x]) count++;
        }
    }
    return count;
}