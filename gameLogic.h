#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <ncurses.h>
#include "config.h"
#include "croc.h"
#include "frog.h"
#include "scoreboard.h"
#include "graphics.h"

#define MAX_CROCS 50
extern CrocData activeCrocs[MAX_CROCS];

// Simboli ASCII per la rana
#define SYMBOL_FROG_1 " @..@"
#define SYMBOL_FROG_2 "(----)"
#define SYMBOL_FROG_3 " ^^^^"

// Se volete gestire granate/proiettili
typedef struct {
    int x, y;
    int active;
} GrenadeData;

typedef struct {
    int x, y;
    int active;
} BulletData;

#define SYMBOL_GRENADE 'G'
#define SYMBOL_BULLET  '*'

// Funzione di collisione bounding box
int checkCollision(int x1, int y1, int w1, int h1,
                   int x2, int y2, int w2, int h2);

// Funzioni di disegno
void drawFrogPos(const FrogData* f);
void drawCrocPos(const CrocData* c);
void drawGrenadePos(const GrenadeData* g);
void drawBulletPos(const BulletData* b);

#endif // GAMELOGIC_H
