#ifndef CROC_H
#define CROC_H

#include "entity.h"
#include "graphics.h"


#define SPRITE_CROC 'C'
#define COCC_WIDTH 9
#define COCC_HEIGHT 3
#define CROCS_PER_LANE 2
#define LANES (RIVER_HEIGHT / COCC_HEIGHT)
#define TOTAL_CROCS (LANES * CROCS_PER_LANE)

// Funzione per inizializzare i coccodrilli nell'area del fiume
void creaCrocodiles(Entity crocs[], int startCol, int endCol, int riverStartRow);

// Funzione per gestire il movimento di un singolo coccodrillo (da eseguire in un processo figlio)
void crocProcess(Entity *croc, int pipeFD);

#endif // CROC_H
