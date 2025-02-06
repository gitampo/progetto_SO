#include "gameLogic.h"

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
void drawGrenadePos(const GrenadeData* g){
    mvaddch(g->y, g->x, SYMBOL_GRENADE);
}

// Disegno proiettile
void drawBulletPos(const BulletData* b){
    mvaddch(b->y, b->x, SYMBOL_BULLET);
}
