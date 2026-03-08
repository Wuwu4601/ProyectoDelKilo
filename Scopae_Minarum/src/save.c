#include "../include/save.h"
#include <stdio.h>

SaveData save = {0, 0, 0, 0, 0, 0};

void loadSave()
{
    FILE* f = fopen("save.dat", "rb");
    if (f) {
        fread(&save, sizeof(SaveData), 1, f);
        fclose(f);
    }
}

void saveGame()
{
    FILE* f = fopen("save.dat", "wb");
    if (f) {
        fwrite(&save, sizeof(SaveData), 1, f);
        fclose(f);
    }
}