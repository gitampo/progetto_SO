#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>
#include "entity.h"  // Aggiungi questa riga per riconoscere il tipo Entity


#define PAVEMENT_HEIGHT 3
#define PAVEMENT_WIDTH  80  // Larghezza dell'area del marciapiede (e del fiume)
#define RIVER_HEIGHT    24   // Altezza del fiume

// Nuove costanti per l'area superiore:
#define MIDDLE_SIDEWALK_HEIGHT 3     // Altezza del marciapiede intermedio
#define TANE_HEIGHT 3               // Altezza delle tane
#define NUM_TANE 5                 // Numero di tane (slot)
#define LARGHEZZA_TANA 8          // Larghezza della tana

#define SYMBOL_TANA_1 "--------"        
#define SYMBOL_TANA_2 "|      |"
#define SYMBOL_TANA_3 "|      |"

// Funzioni di disegno
void drawPavement();
void drawRiver();
void drawMiddlePavement();
<<<<<<< HEAD
void drawVoid();
void drawGrenade(int y, int x);
=======
void drawGrenade(int grenadeX, int grenadeY);
void drawVoid();
>>>>>>> 7dd54a68c627de98bd24141408241068ca4d13df
void drawTane(int statusTane[NUM_TANE]);
// Dichiarazione della funzione per disegnare un bullet
void drawBullet(Entity *bullet);  // Aggiungi questa dichiarazione

// Dichiarazioni delle altre funzioni di disegno
void drawGrenade(int y, int x);
// Altre dichiarazioni di funzioni per il disegno


#endif // GRAPHICS_H
