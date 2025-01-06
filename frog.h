
#ifndef FROG_H
#define FROG_H
#include "graphics.h"
// Definizione delle dimensioni della rana
#define FROG_WIDTH 9
#define FROG_HEIGHT 4

// Struttura che rappresenta la posizione della rana
/*typedef struct {
    int x, y; // Posizione della rana sullo schermo
} Coordinate; */

// Funzioni per la gestione della rana
void initFrog(Coordinate *frog1);
void drawFrog(Coordinate *frog1);
void frog(Coordinate *frog1, int fileds[2]); // Gestisce il movimento della rana

#endif // FROG_H
