#ifndef CROC_H
#define CROC_H

#include "entity.h"
#include "graphics.h"


#define SPRITE_CROC 'C'
#define CROC_WIDTH 12
#define CROC_HEIGHT 3
#define CROCS_PER_LANE 2
#define LANES (RIVER_HEIGHT / CROC_HEIGHT)
#define TOTAL_CROCS (LANES * CROCS_PER_LANE)

void drawCrocodiles(const Entity crocs[], int totalCrocs);

// Funzione per inizializzare i Coccodrilli nell'area del fiume
void creaCrocodiles(Entity crocs[], int startCol, int endCol, int riverStartRow);

// Funzione per gestire il movimento di un singolo Coccodrillo (da eseguire in un processo figlio)
void crocProcess(Entity *croc, int pipeFD, int riverStart);

#endif // CROC_H
