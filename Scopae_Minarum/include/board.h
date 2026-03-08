#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include "../include/config.h"  

extern int board[GRID_H_MAX][GRID_W_MAX];
extern bool revealed[GRID_H_MAX][GRID_W_MAX];
extern bool flagged[GRID_H_MAX][GRID_W_MAX];
extern int currentGridW;
extern int currentGridH;
extern int currentMines;

void clearBoard();
void placeMines();
void generateNumbers();
void revealTile(int x, int y);
void toggleFlag(int x, int y);
int countRevealed();
void setBoardSize(int width, int height, int mines);

#endif