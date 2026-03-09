#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"

extern int board[16][16];
extern int revealed[16][16];
extern int flagged[16][16];
extern int minesPlaced;

void clearBoard();
void placeMines();
void placeMinesAfterClick(int safeX, int safeY);
void generateNumbers();
void revealTile(int x, int y);
void toggleFlag(int x, int y);
int countRevealed();
int countSafeRevealed();

#endif