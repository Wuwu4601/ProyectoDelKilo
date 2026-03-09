#include "../include/board.h"
#include "../include/config.h"
#include "../include/game.h"
#include "../include/save.h"
#include <stdlib.h>
#include <time.h>

int board[16][16];
int revealed[16][16];
int flagged[16][16];
int minesPlaced = 0;

void clearBoard()
{
    int y, x;
    for (y = 0; y < GRID_H_MAX; y++) {
        for (x = 0; x < GRID_W_MAX; x++) {
            board[y][x] = 0;
            revealed[y][x] = 0;
            flagged[y][x] = 0;
        }
    }
    minesPlaced = 0;
}

void placeMines()
{
    int placed, x, y;
    if (minesPlaced) return;
    
    srand(time(NULL) + currentGridW + currentGridH);
    
    placed = 0;
    while (placed < currentMines) {
        x = rand() % currentGridW;
        y = rand() % currentGridH;
        
        if (board[y][x] != -1) {
            board[y][x] = -1;
            placed++;
        }
    }
    
    generateNumbers();
    minesPlaced = 1;
}

void placeMinesAfterClick(int safeX, int safeY)
{
    int placed, x, y, dx, dy;
    if (minesPlaced) return;
    
    srand(time(NULL) + safeX + safeY + roundNumber);
    
    placed = 0;
    while (placed < currentMines) {
        x = rand() % currentGridW;
        y = rand() % currentGridH;
        
        dx = x - safeX;
        dy = y - safeY;
        if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
            continue;
        }
        
        if (board[y][x] != -1) {
            board[y][x] = -1;
            placed++;
        }
    }
    
    generateNumbers();
    minesPlaced = 1;
}

void generateNumbers()
{
    int y, x, dy, dx, count, nx, ny;
    for (y = 0; y < currentGridH; y++) {
        for (x = 0; x < currentGridW; x++) {
            if (board[y][x] == -1) continue;
            
            count = 0;
            for (dy = -1; dy <= 1; dy++) {
                for (dx = -1; dx <= 1; dx++) {
                    nx = x + dx;
                    ny = y + dy;
                    
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
    int dy, dx;
    if (x < 0 || y < 0 || x >= currentGridW || y >= currentGridH) return;
    if (revealed[y][x] || flagged[y][x]) return;
    
    if (!minesPlaced) {
        placeMinesAfterClick(x, y);
    }
    
    revealed[y][x] = 1;
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
        for (dy = -1; dy <= 1; dy++) {
            for (dx = -1; dx <= 1; dx++) {
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
    
    if (flagged[y][x]) {
        minesLeft--;
    } else {
        minesLeft++;
    }
}

int countRevealed()
{
    int count, y, x;
    count = 0;
    for (y = 0; y < currentGridH; y++) {
        for (x = 0; x < currentGridW; x++) {
            if (revealed[y][x]) count++;
        }
    }
    return count;
}