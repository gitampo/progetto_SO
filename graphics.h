#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>
#include "config.h"

typedef struct {
    int width;
    int height;
    int startX;
    int startY;
} Rettangolo;

typedef struct {
    Rettangolo rect;
    int open;
} Tana;

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
// Ritorna indice di tana se la rana entra, altrimenti -1
int checkTane(AreaGioco* area, int frogX, int frogY, int frogW, int frogH);

#endif // GRAPHICS_H
