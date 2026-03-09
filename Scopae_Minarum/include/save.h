#ifndef SAVE_H
#define SAVE_H



typedef struct {
    int bestCalicata;
    int totalRuns;
    int totalMoney;
    int totalScore;
    int bestScore;
    int runsCompleted;
    int totalMinesFound;
    int totalTilesRevealed;
} SaveData;

extern SaveData save;

void loadGame();
void saveGame();

#endif