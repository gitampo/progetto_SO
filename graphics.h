
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>
#include "config.h"

typedef struct {
    int id;  // ID per distinguere le entità
    int y;
    int x;
} Coordinate;

// Rettangolo (marciapiedi, fiume, ecc.)
typedef struct {
    int width;
    int height;
    int startX;
    int startY;
} Rettangolo;

typedef struct {
    Rettangolo rect; 
    int open; // 1 = aperta, 0 = chiusa
} Tana;

// Struttura mappa
typedef struct {
    Rettangolo marciapiedeBasso;
    Rettangolo fiume[NUM_FLUSSI];
    Rettangolo marciapiedeAlto;
    Tana       tane[NUM_TANE];
    int larghezzaSchermo;
    int altezzaSchermo;
} AreaGioco;

void initAreaGioco(AreaGioco* area, int maxX, int maxY);
void disegnaAreaGioco(const AreaGioco* area);
void chiudiTana(Tana* t);

// Controllo se la rana entra in una tana aperta => index, altrimenti -1
int checkTane(AreaGioco* area, int frogX, int frogY, int frogW, int frogH);

#endif