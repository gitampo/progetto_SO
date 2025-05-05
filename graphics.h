#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>
#include "entity.h"  // Aggiungi questa riga per riconoscere il tipo Entity


#define PAVEMENT_HEIGHT 3
#define PAVEMENT_WIDTH  80  // Larghezza dell'area del marciapiede (e del fiume)
#define RIVER_HEIGHT    24   // Altezza del fiume

#define TIMER_BAR_WIDTH 60 // Larghezza della barra del timer

// Nuove costanti per l'area superiore:
#define MIDDLE_SIDEWALK_HEIGHT 3     // Altezza del marciapiede intermedio
#define TANE_HEIGHT 3               // Altezza delle tane
#define NUM_TANE 5                 // Numero di tane (slot)
#define LARGHEZZA_TANA 8          // Larghezza della tana
#define HOLE_ROW  (LINES - RIVER_HEIGHT - PAVEMENT_HEIGHT - TANE_HEIGHT) // Riga delle tane
#define TIMER_WIDTH  60 // Larghezza del timer


#define SYMBOL_TANA_1 "--------"        
#define SYMBOL_TANA_2 "|      |"
#define SYMBOL_TANA_3 "|      |"

// Funzioni di disegno

void drawPavement();
void drawRiver();
void drawMiddlePavement();
void drawVoid();
void drawTane(int statusTane[NUM_TANE]);
void drawBullet(Entity *bullet);  
void drawGrenade(Entity *grenade);  
// Timer/progress-bar sotto il marciapiede inferiore
void timerBarInit(int max_time, int row, int colStart, int width, int *lives, Entity *frog, int frogStartY, int frogStartX, int toFrogFD);
void timerBarUpdate();
void timerProcess(int writeFd, int duration);
void DrawTimer();


#endif // GRAPHICS_H
