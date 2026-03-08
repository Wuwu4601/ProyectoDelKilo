#include "../include/game.h"
#include "../include/board.h"
#include "../include/config.h"
#include "../include/save.h"
#include "raylib.h"

GameState gameState = STATE_MENU;
int level = 1;
int score = 0;
int baseScore = 0;
int mult = 1;
int minesLeft = MINES_CATITO;
float gameTime = 0;
bool gameOver = false;
bool gameWon = false;
int currentLives = START_LIVES;
int maxLives = START_LIVES;
RoundType roundType = ROUND_CATITO;
int targetScore = SCORE_CATITO;
int calicataLevel = 1;
int money = 0;
int totalMinesFound = 0;
int totalTilesRevealed = 0;
int runsCompleted = 0;
int bestCalicata = 0;
int roundNumber = 1;
bool firstClick = true;
bool sondeoBeaten = false;
bool cataBeaten = false;
bool calicataBeaten = false;

// Variables para End of Round
int timeBonus = 0;
int totalScore = 0;
int roundMoney = 0;
bool roundCompleted = false;
float timeSaved = 0;
int parTimeMultiplier = 1;
int currentParTime = 0;

int currentGridW = GRID_W_CATITO;
int currentGridH = GRID_H_CATITO;
int currentMines = MINES_CATITO;

int getParTime()
{
    switch (roundType) {
        case ROUND_CATITO: return PAR_TIME_CATITO;
        case ROUND_SONDEO: return PAR_TIME_SONDEO;
        case ROUND_CATA: return PAR_TIME_CATA;
        case ROUND_CALICATA: return PAR_TIME_CALICATA;
        default: return 60;
    }
}

int countSafeRevealed()
{
    int count = 0;
    for (int y = 0; y < currentGridH; y++) {
        for (int x = 0; x < currentGridW; x++) {
            if (revealed[y][x] && board[y][x] != -1) {
                count++;
            }
        }
    }
    return count;
}

void initGame()
{
    switch (roundType) {
        case ROUND_CATITO:
            currentGridW = GRID_W_CATITO;
            currentGridH = GRID_H_CATITO;
            currentMines = MINES_CATITO;
            targetScore = SCORE_CATITO;
            break;
        case ROUND_SONDEO:
            currentGridW = GRID_W_SONDEO;
            currentGridH = GRID_H_SONDEO;
            currentMines = MINES_SONDEO;
            targetScore = SCORE_SONDEO;
            break;
        case ROUND_CATA:
            currentGridW = GRID_W_CATA;
            currentGridH = GRID_H_CATA;
            currentMines = MINES_CATA;
            targetScore = SCORE_CATA;
            break;
        case ROUND_CALICATA:
            currentGridW = GRID_W_CALICATA;
            currentGridH = GRID_H_CALICATA;
            currentMines = MINES_CALICATA;
            targetScore = SCORE_CALICATA;
            break;
    }
    
    clearBoard();
    placeMines();
    generateNumbers();
    
    minesLeft = currentMines;
    gameTime = 0;
    gameOver = false;
    gameWon = false;
    baseScore = 0;
    score = 0;
    mult = 1;
    currentLives = maxLives;
    
    totalMinesFound = 0;
    totalTilesRevealed = 0;
    firstClick = true;
    
    timeBonus = 0;
    totalScore = 0;
    roundMoney = 0;
    roundCompleted = false;
    timeSaved = 0;
}

void updateGame()
{
    if (!gameOver) {
        gameTime += GetFrameTime();
        score = baseScore * mult;
        checkWinCondition();
    }
}

void addScore(int points)
{
    baseScore += points;
    score = baseScore * mult;
}

void loseLife()
{
    currentLives--;
    
    if (currentLives <= 0) {
        gameOver = true;
        
        if (calicataLevel > bestCalicata) {
            bestCalicata = calicataLevel;
        }
        save.bestCalicata = bestCalicata;
        saveGame();
        
        gameState = STATE_GAMEOVER;
    }
}

void checkWinCondition()
{
    int safeRevealed = countSafeRevealed();
    int totalSafe = (currentGridW * currentGridH) - currentMines;
    bool boardCompleted = (safeRevealed >= totalSafe);
    
    bool targetReached = (score >= targetScore);
    
    if ((boardCompleted || targetReached) && !gameOver && !gameWon) {
        gameWon = true;
        gameOver = true;
        
        if (roundType == ROUND_SONDEO) sondeoBeaten = true;
        if (roundType == ROUND_CATA) cataBeaten = true;
        if (roundType == ROUND_CALICATA) {
            calicataBeaten = true;
            calicataLevel++;
            if (calicataLevel > 10) {
                calicataLevel = 10;
            }
            sondeoBeaten = false;
            cataBeaten = false;
        }
        
        int parTime = getParTime();
        currentParTime = parTime;
        
        if (gameTime < parTime) {
            timeSaved = parTime - gameTime;
            timeBonus = (int)(timeSaved * parTimeMultiplier + 0.5f);
        } else {
            timeSaved = 0;
            timeBonus = 0;
        }
        
        totalScore = score + timeBonus;
        roundMoney = getRoundReward(roundType);
        roundCompleted = (totalScore >= targetScore);
        
        runsCompleted++;
        save.runsCompleted = runsCompleted;
        saveGame();
        
        gameState = STATE_END_OF_ROUND;
    }
}

void nextRound()
{
    if (roundType == ROUND_CATITO) {
        roundType = ROUND_SONDEO;
    } else if (roundType == ROUND_SONDEO) {
        roundType = ROUND_CATA;
    } else if (roundType == ROUND_CATA) {
        roundType = ROUND_CALICATA;
    } else {
        roundType = ROUND_SONDEO;
    }
    
    roundNumber++;
    initGame();
    currentLives = maxLives;
    gameState = STATE_PLAYING;
}

void goToSondeo()
{
    roundType = ROUND_SONDEO;
    roundNumber++;
    initGame();
    currentLives = maxLives;
    gameState = STATE_PLAYING;
}

void goToCata()
{
    roundType = ROUND_CATA;
    roundNumber++;
    initGame();
    currentLives = maxLives;
    gameState = STATE_PLAYING;
}

void goToNextAvailable()
{
    if (!sondeoBeaten) {
        goToSondeo();
    } else if (!cataBeaten) {
        goToCata();
    } else if (!calicataBeaten) {
        roundType = ROUND_CALICATA;
        roundNumber++;
        initGame();
        currentLives = maxLives;
        gameState = STATE_PLAYING;
    } else {
        calicataBeaten = false;
        roundType = ROUND_SONDEO;
        roundNumber++;
        initGame();
        currentLives = maxLives;
        gameState = STATE_PLAYING;
    }
}

const char* getRoundTypeName(RoundType type)
{
    switch (type) {
        case ROUND_CATITO: return "CATITO";
        case ROUND_SONDEO: return "SONDEO";
        case ROUND_CATA: return "CATA";
        case ROUND_CALICATA: return "CALICATA";
        default: return "UNKNOWN";
    }
}

Color getRoundTypeColor(RoundType type)
{
    switch (type) {
        case ROUND_CATITO: return (Color){100, 255, 100, 255};
        case ROUND_SONDEO: return (Color){100, 200, 255, 255};
        case ROUND_CATA: return (Color){255, 200, 100, 255};
        case ROUND_CALICATA: return (Color){255, 100, 100, 255};
        default: return WHITE;
    }
}

int getRoundReward(RoundType type)
{
    switch (type) {
        case ROUND_CATITO: return 50;
        case ROUND_SONDEO: return 100;
        case ROUND_CATA: return 250;
        case ROUND_CALICATA: return 500;
        default: return 100;
    }
}