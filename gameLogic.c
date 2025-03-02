#include "gameLogic.h"

#include "gameLogic.h"

// Definizione della variabile globale
CrocData activeCrocs[MAX_CROCS];

// Funzione bounding box collision
int checkCollision(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 + w1 <= x2) return 0;
    if(x2 + w2 <= x1) return 0;
    if(y1 + h1 <= y2) return 0;
    if(y2 + h2 <= y1) return 0;
    return 1;
}

// Disegno della rana
void drawFrogPos(const FrogData* f){
    mvprintw(f->y,   f->x, SYMBOL_FROG_1);
    mvprintw(f->y+1, f->x, SYMBOL_FROG_2);
    mvprintw(f->y+2, f->x, SYMBOL_FROG_3);
}

// Disegno coccodrillo
void drawCrocPos(const CrocData* c){
    //  Esempio: 2 righe di "CCCC"
    for(int row=0; row < c->height; row++){
        mvprintw(c->y + row, c->x, "CCCC");
    }
}

// Disegno granata
void drawGrenadePos(const GrenadeData* g) {
    if (g->active) {
        mvaddch(g->y, g->x, SYMBOL_GRENADE);
    }
}

// Disegno proiettile
void drawBulletPos(const BulletData* b){
    mvaddch(b->y, b->x, SYMBOL_BULLET);
}

int checkFrogInTana(const Tana* tana, const FrogData* frog) {
    // Coordinate bounding box della tana
    int tanaLeft   = tana->rect.startX;
    int tanaRight  = tana->rect.startX + tana->rect.width;
    int tanaTop    = tana->rect.startY;
    int tanaBottom = tana->rect.startY + tana->rect.height;

    // Coordinate bounding box della rana
    int frogLeft   = frog->x;
    int frogRight  = frog->x + frog->width;
    int frogTop    = frog->y;
    int frogBottom = frog->y + frog->height;

    // Verifica se la rana è completamente dentro la tana
    // Puoi cambiare logica se vuoi solo un punto della rana all'interno.
    if (frogLeft >= tanaLeft && frogRight <= tanaRight &&
        frogTop >= tanaTop   && frogBottom <= tanaBottom) {
        return 1; // La rana è dentro la tana
    }
    return 0; // Non è nella tana
}

int checkFrogInTana(const Tana* tana, const FrogData* frog) {
    // Coordinate bounding box della tana
    int tanaLeft   = tana->rect.startX;
    int tanaRight  = tana->rect.startX + tana->rect.width;
    int tanaTop    = tana->rect.startY;
    int tanaBottom = tana->rect.startY + tana->rect.height;

    // Coordinate bounding box della rana
    int frogLeft   = frog->x;
    int frogRight  = frog->x + frog->width;
    int frogTop    = frog->y;
    int frogBottom = frog->y + frog->height;

    // Verifica se la rana è completamente dentro la tana
    // Puoi cambiare logica se vuoi solo un punto della rana all'interno.
    if (frogLeft >= tanaLeft && frogRight <= tanaRight &&
        frogTop >= tanaTop   && frogBottom <= tanaBottom) {
        return 1; // La rana è dentro la tana
    }
    return 0; // Non è nella tana
}
