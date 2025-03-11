#ifndef FROG_H
#define FROG_H

#include "graphics.h"  // Se serve usare Coordinates o altre strutture comuni

// Larghezza e altezza dello sprite della rana
#define FROG_WIDTH  8
#define FROG_HEIGHT 3
#define FROG_JUMP_X 2
#define FROG_JUMP_Y FROG_HEIGHT


// Tre righe di sprite della rana
#define SYMBOL_FROG_1 "  @..@  "
#define SYMBOL_FROG_2 " (----)"
#define SYMBOL_FROG_3 "  ~~~~  "

// Struttura che memorizza la posizione "top-left" della rana sullo schermo.
typedef struct {
    int y; // Riga di riferimento
    int x; // Colonna di riferimento
} FrogPos;

// Funzioni di disegno
void drawFrog(const FrogPos *frog);
void clearFrog(const FrogPos *frog);

// Funzione per muovere la rana in base al tasto premuto (freccia su/giu/sinistra/destra).
void moveFrog(FrogPos *frog, int inputKey);

#endif // FROG_H
