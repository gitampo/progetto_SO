#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <fcntl.h> // Per O_NONBLOCK
#include "graphics.h"
#include "frog.h"
#include "croc.h"
#include "entity.h"
#include "collision.h"


extern int taneOccupate[NUM_TANE];  // Permette a frog.c di usare la variabile

int isFrogInTana(const Entity *frog) {
    int taneRow = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    double slotWidth = (double)PAVEMENT_WIDTH / NUM_TANE + 0.5;

    if (frog->y == taneRow) { // Controlla se la rana è sulla riga delle tane
        for (int i = 0; i < NUM_TANE; i++) {
            int tanaStartCol = startCol + i * (int)slotWidth + (((int)slotWidth - LARGHEZZA_TANA) / 2);
            if (frog->x >= tanaStartCol && frog->x < tanaStartCol + LARGHEZZA_TANA) {
                return i; // Restituisce l'indice della tana
            }
        }
    }
    return -1;  // Nessuna tana trovata
}
