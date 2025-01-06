#include "frog.h"
#include <ncurses.h>

// Disegno ASCII della rana
const char *frog1[] = {
    "         @..@",
    "        (----)",
    "       ( >__< )",
    "       ^^ ~~ ^^"
};

// Funzione che inizializza la posizione della rana
void initFrog(Coordinate *frog1) {
    frog1->x = COLS / 2 - FROG_WIDTH / 2; // Posiziona la rana al centro orizzontalmente
    frog1->y = LINES - FROG_HEIGHT - 1;  // Posiziona la rana al fondo dello schermo
}

void drawFrog(Coordinate *frogPosition) {
    // Ciclo per disegnare ogni linea dell'arte ASCII della rana
    for (int i = 0; i < FROG_HEIGHT; i++) {
        mvprintw(frogPosition->y + i, frogPosition->x, frog1[i]);
    }
    refresh();
}

// Funzione per muovere la rana in base ai tasti direzionali
void frog(Coordinate *frog1, int fileds[2]) {

    while(1){

    switch (getch()) {
        case KEY_UP:
            if (frog1->y > 1) frog1->y -= 1;
            break;
        case KEY_DOWN:
            if (frog1->y < LINES - FROG_HEIGHT - 1) frog1->y += 1;
            break;
        case KEY_LEFT:
            if (frog1->x > 1) frog1->x -= 1;
            break;
        case KEY_RIGHT:
            if (frog1->x < COLS - FROG_WIDTH - 1) frog1->x += 1;
            break;
    }

    write(fileds[1], frog1, sizeof(Coordinate));
    usleep(10000);
    } 
}
