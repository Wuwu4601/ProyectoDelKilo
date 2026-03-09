#include "../include/save.h"
#include "../include/game.h"  /* ✅ Agregar esto para acceder a las variables globales */
#include <stdio.h>

SaveData save;

void loadGame()
{
    FILE *f;
    f = fopen("save.dat", "rb");
    if (f) {
        fread(&save, sizeof(SaveData), 1, f);
        fclose(f);
        
        /* ✅ Ahora estas variables son accesibles */
        money = save.totalMoney;
        bestCalicata = save.bestCalicata;
        runsCompleted = save.runsCompleted;
    } else {
        save.bestCalicata = 0;
        save.totalRuns = 0;
        save.totalMoney = 0;
        save.totalScore = 0;
        save.bestScore = 0;
        save.runsCompleted = 0;
        save.totalMinesFound = 0;
        save.totalTilesRevealed = 0;
        
        money = 0;
        bestCalicata = 0;
        runsCompleted = 0;
    }
}

void saveGame()
{
    FILE *f;
    f = fopen("save.dat", "wb");
    if (f) {
        fwrite(&save, sizeof(SaveData), 1, f);
        fclose(f);
    }
}