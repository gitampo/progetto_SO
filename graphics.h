#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>

#define PAVEMENT_HEIGHT 3
#define PAVEMENT_WIDTH  84   // Larghezza dell'area del marciapiede (e del fiume)
#define RIVER_HEIGHT    24   // Altezza del fiume

// Nuove costanti per l'area superiore:
#define MIDDLE_SIDEWALK_HEIGHT 3   // Altezza del marciapiede intermedio
#define TANE_HEIGHT 3              // Altezza delle tane
#define NUM_TANE 5                 // Numero di tane (slot)
#define LARGHEZZA_TANA 9          // Larghezza della tana

#define SYMBOL_TANA_1 "---------"        
#define SYMBOL_TANA_2 "|       |"
#define SYMBOL_TANA_3 "|       |"

// Funzioni di disegno
void drawPavement();
void drawRiver();
void drawMiddlePavement();
void drawTane(int statusTane[NUM_TANE]);

#endif // GRAPHICS_H
