#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <ncurses.h>
#include "config.h"
#include "croc.h"
#include "frog.h"
#include "grenade.h"
#include "bullet.h"
#include "scoreboard.h"
#include "graphics.h"

#define MAX_CROCS 50

// Struttura di "messaggio" unificato, se vuoi
// In alternativa puoi usare direttamente FrogData/CrocData
// ...
// extern CrocData activeCrocs[MAX_CROCS];
// extern int nCrocs;

// Funzione di collisione bounding box
int checkCollision(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);

// Funzioni di disegno (il padre le userà per disegnare)
void drawFrogPos(const FrogData* f);
void drawCrocPos(const CrocData* c);
void drawGrenadePos(const GrenadeData* g);
void drawBulletPos(const BulletData* b);

#endif // GAMELOGIC_H
