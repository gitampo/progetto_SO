#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <fcntl.h> // Per O_NONBLOCK
#include "graphics.h"
#include "croc.h"
#include "entity.h"
#include "collision.h"
#include "frog.h"

int isFrogInTana(const Entity *frog) {
    int taneRow = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int slotWidth = PAVEMENT_WIDTH / NUM_TANE;

    if (frog->y == taneRow) { // Controlla se la rana è sulla riga delle tane
        for (int i = 0; i < NUM_TANE; i++) {
            int tanaStartCol = startCol + i * slotWidth + ((slotWidth - LARGHEZZA_TANA) / 2);
            if (
            inBetween(frog->x, tanaStartCol, tanaStartCol + LARGHEZZA_TANA - 1) ||
            inBetween(frog->x + FROG_WIDTH - 1, tanaStartCol, tanaStartCol + LARGHEZZA_TANA - 1)
        ) return i;
            /*
            if (frog->x >= tanaStartCol && frog->x < tanaStartCol + LARGHEZZA_TANA) {
                return i; // Restituisce l'indice della tana
            }   
            if(frog->x + FROG_WIDTH >= tanaStartCol && frog->x + FROG_WIDTH < tanaStartCol + LARGHEZZA_TANA) {
                return i; // Restituisce l'indice della tana
            }
                */
        }
    }
    return -1;  // Nessuna tana trovata
}

bool inBetween(int value, int min, int max) {
    return (value >= min && value <= max);
}

bool collision(Entity frog, Entity tana){
    return frog.y == tana.y && 
           inBetween(frog.x, tana.x, tana.x + LARGHEZZA_TANA) &&
           inBetween(frog.x + FROG_WIDTH, tana.x, tana.x + LARGHEZZA_TANA);
}


