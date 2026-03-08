#ifndef SAVE_H
#define SAVE_H

typedef struct {
    int bestScore;
    int runsCompleted;
    int totalMinesFound;
    int totalTilesRevealed;
    int bestCalicata;
    int totalMoneyEarned;
} SaveData;

void loadSave();
void saveGame();
extern SaveData save;

#endif