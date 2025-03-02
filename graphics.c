#include "graphics.h"

#define CHAR_MARCIAPIEDE '='
#define CHAR_FIUME       ' '
#define CHAR_ERBA        '@'
#define CHAR_TANA_APERTA '0'
#define CHAR_TANA_CHIUSA ' '

static void riempiRettangolo(const Rettangolo* r, char ch){
    for(int row=0; row<r->height; row++){
        move(r->startY + row, r->startX);
        for(int col=0; col<r->width; col++){
            addch(ch);
        }
    }
}

void initAreaGioco(AreaGioco* area, int maxX, int maxY) {
    int gameWidth = maxX / 2;   // Riduce la larghezza della schermata di gioco
    int gameHeight = maxY - 2;  // Aumenta l'altezza lasciando spazio per la scoreboard

    int startX = (maxX - gameWidth) / 2;  // Centro orizzontalmente
    int startY = 1;  // Inizia dall'alto lasciando spazio per la scoreboard

    area->larghezzaSchermo = gameWidth;
    area->altezzaSchermo = gameHeight;

    // Marciapiede inferiore (ora più alto per contenere la rana)
    area->marciapiedeBasso.height = 4;  // Prima era 2, ora è 4 per sicurezza
    area->marciapiedeBasso.width = gameWidth;
    area->marciapiedeBasso.startX = startX;
    area->marciapiedeBasso.startY = startY + gameHeight - area->marciapiedeBasso.height;

    // Fiume con più spazio verticale
    int flussoH = 3;  // Altezza maggiore per il fiume
    for (int i = 0; i < NUM_FLUSSI; i++) {
        area->fiume[i].width = gameWidth;
        area->fiume[i].height = flussoH;
        area->fiume[i].startX = startX;
        area->fiume[i].startY = area->marciapiedeBasso.startY - (i + 1) * flussoH;
    }

    // Marciapiede superiore (ora più alto)
    area->marciapiedeAlto.height = 4;  // Prima era 2, ora è 4 per sicurezza
    area->marciapiedeAlto.width = gameWidth;
    area->marciapiedeAlto.startX = startX;
    area->marciapiedeAlto.startY = area->fiume[NUM_FLUSSI - 1].startY - area->marciapiedeAlto.height;

    // Tane centrate
    int tanaW = gameWidth / (NUM_TANE * 2);
    int tanaH = 3;
    for (int i = 0; i < NUM_TANE; i++) {
        area->tane[i].rect.width = tanaW;
        area->tane[i].rect.height = tanaH;
        area->tane[i].rect.startX = startX + i * (gameWidth / NUM_TANE) + ((gameWidth / NUM_TANE - tanaW) / 2);
        area->tane[i].rect.startY = area->marciapiedeAlto.startY - tanaH;
        area->tane[i].open = 1;
    }
}



void disegnaAreaGioco(const AreaGioco* area) {
    // Disegna marciapiede inferiore
    riempiRettangolo(&area->marciapiedeBasso, '=');

    // Disegna fiume
    for (int i = 0; i < NUM_FLUSSI; i++) {
        riempiRettangolo(&area->fiume[i], '~');
    }

    // Disegna marciapiede superiore
    riempiRettangolo(&area->marciapiedeAlto, '@');

    // Disegna tane
    for (int i = 0; i < NUM_TANE; i++) {
        if (area->tane[i].open)
            riempiRettangolo(&area->tane[i].rect, 'O');  // Tana aperta
        else
            riempiRettangolo(&area->tane[i].rect, ' ');  // Tana chiusa
    }

    refresh();
}


void chiudiTana(Tana* t){
    t->open=0;
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
            int fx2 = fx1 + frogW;
            int fy2 = fy1 + frogH;

            if(fx2>tx1 && fx1<tx2 && fy2>ty1 && fy1<ty2){
                return i; // indice tana
            }
        }
    }
    return -1;
}
