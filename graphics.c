#include "graphics.h"

#define CHAR_MARCIAPIEDE '='
#define CHAR_FIUME       ' '
#define CHAR_ERBA        '@'
#define CHAR_TANA_APERTA '0'
#define CHAR_TANA_CHIUSA ' '

static void riempiRettangolo(const Rettangolo* r, char ch){
    for(int row = 0; row < r->height; row++){
        move(r->startY + row, r->startX);
        for(int col = 0; col < r->width; col++){
            addch(ch);
        }
    }
}

/**
 * initAreaGioco:
 *  - Riduce la larghezza/altezza di default,
 *    per evitare che si disegni fuori schermo su terminal piccoli.
 */
void initAreaGioco(AreaGioco* area, int maxX, int maxY) {
    // Forziamo dimensioni massime ridotte, così la rana si vede
    int gameWidth  = (maxX >= 60) ? 50 : maxX - 2;   
    int gameHeight = (maxY >= 24) ? 18 : maxY - 5;

    // Evita che vadano sotto zero
    if (gameWidth < 10)  gameWidth = 10;
    if (gameHeight < 10) gameHeight = 10;

    int startX = (maxX - gameWidth) / 2;  // centra orizzontalmente
    int startY = 1;  // Riga 1, scoreboard su riga 0

    area->larghezzaSchermo = gameWidth;
    area->altezzaSchermo   = gameHeight;

    // Marciapiede inferiore
    area->marciapiedeBasso.height = 4;
    area->marciapiedeBasso.width  = gameWidth;
    area->marciapiedeBasso.startX = startX;
    area->marciapiedeBasso.startY =
        startY + gameHeight - area->marciapiedeBasso.height;

    // Fiume
    int flussoH = 3;
    for (int i = 0; i < NUM_FLUSSI; i++) {
        area->fiume[i].width  = gameWidth;
        area->fiume[i].height = flussoH;
        area->fiume[i].startX = startX;
        area->fiume[i].startY =
            area->marciapiedeBasso.startY - (i + 1) * flussoH;
    }

    // Marciapiede superiore
    area->marciapiedeAlto.height = 4;
    area->marciapiedeAlto.width  = gameWidth;
    area->marciapiedeAlto.startX = startX;
    area->marciapiedeAlto.startY =
        area->fiume[NUM_FLUSSI - 1].startY - area->marciapiedeAlto.height;

    // Tane
    int tanaW = gameWidth / (NUM_TANE * 2);
    int tanaH = 3;
    if (tanaW < 2) tanaW = 2;
    for (int i = 0; i < NUM_TANE; i++) {
        area->tane[i].rect.width  = tanaW;
        area->tane[i].rect.height = tanaH;
        area->tane[i].rect.startX =
            startX + i * (gameWidth / NUM_TANE) + 
            ((gameWidth / NUM_TANE - tanaW) / 2);
        area->tane[i].rect.startY = area->marciapiedeAlto.startY - tanaH;
        area->tane[i].open       = 1;
    }
}

void disegnaAreaGioco(const AreaGioco* area) {
    // Marciapiede inferiore
    riempiRettangolo(&area->marciapiedeBasso, CHAR_MARCIAPIEDE);

    // Fiume
    for (int i = 0; i < NUM_FLUSSI; i++) {
        riempiRettangolo(&area->fiume[i], CHAR_FIUME);
    }

    // Marciapiede superiore
    riempiRettangolo(&area->marciapiedeAlto, CHAR_ERBA);

    // Tane
    for (int i = 0; i < NUM_TANE; i++) {
        if (area->tane[i].open)
            riempiRettangolo(&area->tane[i].rect, CHAR_TANA_APERTA);
        else
            riempiRettangolo(&area->tane[i].rect, CHAR_TANA_CHIUSA);
    }
    refresh();
}

void chiudiTana(Tana* t){
    t->open = 0;
    for(int r=0; r < t->rect.height; r++){
        move(t->rect.startY + r, t->rect.startX);
        for(int c=0; c < t->rect.width; c++){
            addch(CHAR_TANA_CHIUSA);
        }
    }
    refresh();
}

int checkTane(AreaGioco* area, int frogX, int frogY, int frogW, int frogH){
    for(int i = 0; i < NUM_TANE; i++){
        if(area->tane[i].open){
            int tx1 = area->tane[i].rect.startX;
            int ty1 = area->tane[i].rect.startY;
            int tx2 = tx1 + area->tane[i].rect.width;
            int ty2 = ty1 + area->tane[i].rect.height;

            int fx1 = frogX;
            int fy1 = frogY;
            int fx2 = fx1 + frogW;
            int fy2 = fy1 + frogH;

            if(fx2 > tx1 && fx1 < tx2 && fy2 > ty1 && fy1 < ty2){
                return i; // indice della tana in cui è entrata la rana
            }
        }
    }
    return -1;
}
