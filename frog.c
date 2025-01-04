#include "frog.h"
#include <ncurses.h>

// Disegno ASCII della rana
const char *frog[] = {
    "         @..@",
    "        (----)",
    "       ( >__< )",
    "       ^^ ~~ ^^"
};

// Funzione che inizializza la posizione della rana
void initFrog(Coordinate *frog) {
    frog->x = COLS / 2 - FROG_WIDTH / 2; // Posiziona la rana al centro orizzontalmente
    frog->y = LINES - FROG_HEIGHT - 1;  // Posiziona la rana al fondo dello schermo
}

void drawFrog(Coordinate *frogPosition) {
    // Ciclo per disegnare ogni linea dell'arte ASCII della rana
    for (int i = 0; i < FROG_HEIGHT; i++) {
        mvprintw(frogPosition->y + i, frogPosition->x, frog[i]);
    }
    refresh();
}

// Funzione per muovere la rana in base ai tasti direzionali
void moveFrog(Coordinate *frog, int direction) {
    switch (direction) {
        case KEY_UP:
            if (frog->y > 1) frog->y -= 1;
            break;
        case KEY_DOWN:
            if (frog->y < LINES - FROG_HEIGHT - 1) frog->y += 1;
            break;
        case KEY_LEFT:
            if (frog->x > 1) frog->x -= 1;
            break;
        case KEY_RIGHT:
            if (frog->x < COLS - FROG_WIDTH - 1) frog->x += 1;
            break;
    }
}
