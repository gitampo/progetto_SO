#include "graphics.h"

#define CHAR_MARCIAPIEDE '='
#define CHAR_FIUME       '~'
#define CHAR_ERBA        '@'
#define CHAR_TANA_APERTA ' '
#define CHAR_TANA_CHIUSA '#'

static void riempiRettangolo(const Rettangolo* r, char ch){
    for(int row=0; row<r->height; row++){
        move(r->startY + row, r->startX);
        for(int col=0; col<r->width; col++){
            addch(ch);
        }
    }
}

void initAreaGioco(AreaGioco* area, int maxX, int maxY){
    area->larghezzaSchermo = maxX;
    area->altezzaSchermo   = maxY;

    // marciapiede basso
    area->marciapiedeBasso.height = 3;
    area->marciapiedeBasso.width  = maxX;
    area->marciapiedeBasso.startX = 0;
    area->marciapiedeBasso.startY = maxY - 3;

    // fiume con 8 flussi
    int flussoH = 3;
    for(int i=0; i<NUM_FLUSSI; i++){
        area->fiume[i].width  = maxX;
        area->fiume[i].height = flussoH;
        area->fiume[i].startX = 0;
        area->fiume[i].startY = area->marciapiedeBasso.startY - (i+1)*flussoH;
    }
    // marciapiede alto (erba)
    area->marciapiedeAlto.height = 3;
    area->marciapiedeAlto.width  = maxX;
    area->marciapiedeAlto.startX = 0;
    area->marciapiedeAlto.startY = area->fiume[NUM_FLUSSI-1].startY - 3;

    // tane
    int tanaW = maxX/(NUM_TANE*2);
    int tanaH = 3;
    for(int i=0; i<NUM_TANE; i++){
        area->tane[i].rect.width  = tanaW;
        area->tane[i].rect.height = tanaH;
        area->tane[i].rect.startX = i*(maxX/NUM_TANE) + ((maxX/NUM_TANE - tanaW)/2);
        area->tane[i].rect.startY = area->marciapiedeAlto.startY - tanaH;
        area->tane[i].open = 1;
    }
}

void disegnaAreaGioco(const AreaGioco* area){
    // Basso
    riempiRettangolo(&area->marciapiedeBasso, CHAR_MARCIAPIEDE);
    // Fiume
    for(int i=0; i<NUM_FLUSSI; i++){
        riempiRettangolo(&area->fiume[i], CHAR_FIUME);
    }
    // Alto
    riempiRettangolo(&area->marciapiedeAlto, CHAR_ERBA);
    // Tane
    for(int i=0; i<NUM_TANE; i++){
        if(area->tane[i].open)
            riempiRettangolo(&area->tane[i].rect, CHAR_TANA_APERTA);
        else
            riempiRettangolo(&area->tane[i].rect, CHAR_TANA_CHIUSA);
    }
    refresh();
}

void chiudiTana(Tana* t){
    t->open = 0;
    for(int r=0; r<t->rect.height; r++){
        move(t->rect.startY + r, t->rect.startX);
        for(int c=0; c<t->rect.width; c++){
            addch(CHAR_TANA_CHIUSA);
        }
    }
    refresh();
}

int checkTane(AreaGioco* area, int frogX, int frogY, int frogW, int frogH){
    for(int i=0; i<NUM_TANE; i++){
        if(area->tane[i].open){
            int tx1 = area->tane[i].rect.startX;
            int ty1 = area->tane[i].rect.startY;
            int tx2 = tx1 + area->tane[i].rect.width;
            int ty2 = ty1 + area->tane[i].rect.height;
            
            int fx1 = frogX;
            int fy1 = frogY;
            int fx2 = fx1+frogW;
            int fy2 = fy1+frogH;

            if(fx2>tx1 && fx1<tx2 && fy2>ty1 && fy1<ty2){
                // collisione => la rana è entrata
                return i;
            }
        }
    }
    return -1;
}