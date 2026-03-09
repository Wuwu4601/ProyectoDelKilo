#ifndef GAME_H
#define GAME_H

#include "raylib.h"

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAMEOVER,
    STATE_WIN,
    STATE_SHOP,
    STATE_STATS,
    STATE_PAUSE,
    STATE_OPTIONS,
    STATE_CATA_SELECT,
    STATE_END_OF_ROUND
} GameState;

typedef enum {
    ROUND_CATITO,
    ROUND_SONDEO,
    ROUND_CATA,
    ROUND_CALICATA
} RoundType;

extern GameState gameState;
extern int level;
extern int score;
extern int baseScore;
extern int mult;
extern int minesLeft;
extern float gameTime;
extern int gameOver;
extern int gameWon;
extern int currentLives;
extern int maxLives;
extern RoundType roundType;
extern int targetScore;
extern int calicataLevel;
extern int money;
extern int totalMinesFound;
extern int totalTilesRevealed;
extern int runsCompleted;
extern int bestCalicata;
extern int roundNumber;
extern int firstClick;
extern int currentGridW;
extern int currentGridH;
extern int currentMines;
extern int sondeoBeaten;
extern int cataBeaten;
extern int calicataBeaten;

extern int timeBonus;
extern int totalScore;
extern int roundMoney;
extern int roundCompleted;
extern float timeSaved;
extern int parTimeMultiplier;
extern int currentParTime;

void initGame();
void updateGame();
void checkWinCondition();
void nextRound();
void addScore(int points);
void loseLife();
const char* getRoundTypeName(RoundType type);
Color getRoundTypeColor(RoundType type);
int getRoundReward(RoundType type);
void goToSondeo();
void goToCata();
void goToNextAvailable();
int getParTime();
int countSafeRevealed();

#endif