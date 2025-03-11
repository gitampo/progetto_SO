#include "frog.h"
#include <ncurses.h>

// Disegna la rana come 3 righe di testo
void drawFrog(const FrogPos *frog) {
    attron(COLOR_PAIR(1));
    mvprintw(frog->y,     frog->x, SYMBOL_FROG_1);
    mvprintw(frog->y + 1, frog->x, SYMBOL_FROG_2);
    mvprintw(frog->y + 2, frog->x, SYMBOL_FROG_3);
    attroff(COLOR_PAIR(1));
}

// Cancella la rana disegnando spazi al posto dello sprite
void clearFrog(const FrogPos *frog) {
    for (int i = 0; i < FROG_HEIGHT; i++) {
        // Stampa 7 spazi (FROG_WIDTH) alla riga y+i, colonna x
        mvprintw(frog->y + i, frog->x, "       ");
    }
}

// Muove la rana in base al tasto premuto (freccia su/giu/sinistra/destra).
void moveFrog(FrogPos *frog, int inputKey) {
    switch (inputKey) {
        case KEY_UP:
            // Verifica di non uscire in alto
            if (frog->y >= FROG_JUMP_Y) {
                frog->y -= FROG_JUMP_Y;
            } else {
                // Se non vuoi farla uscire fuori, la blocchi a 0
                frog->y = 0;
            }
            break;

        case KEY_DOWN:
            // Verifica di non superare il limite in basso
            if (frog->y + FROG_JUMP_Y <= LINES - FROG_HEIGHT) {
                frog->y += FROG_JUMP_Y;
            } else {
                frog->y = LINES - FROG_HEIGHT;  
            }
            break;

        case KEY_LEFT:
            if (frog->x >= FROG_JUMP_X) {
                frog->x -= FROG_JUMP_X;
            } else {
                frog->x = 0;
            }
            break;

        case KEY_RIGHT:
            if (frog->x + FROG_JUMP_X <= COLS - FROG_WIDTH) {
                frog->x += FROG_JUMP_X;
            } else {
                frog->x = COLS - FROG_WIDTH;
            }
            break;

        default:
            // Nessun movimento
            break;
    }
}
