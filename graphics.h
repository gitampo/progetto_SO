#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>

#define PAVEMENT_HEIGHT 3
#define PAVEMENT_WIDTH  84   // Larghezza dell'area del marciapiede (e del fiume)
#define RIVER_HEIGHT    24   // Altezza del fiume


// Funzioni di disegno
void drawPavement();
void drawRiver();

#endif // GRAPHICS_H