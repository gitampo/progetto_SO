#include "grenade.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

// Inizializza una granata con posizione e direzione
void initGrenade(Grenade *grenade, int x, int y, int direction) {
    grenade->x = x;
    grenade->y = y;
    grenade->active = true;
    grenade->direction = direction;
}

// Funzione del processo per muovere una granata
void moveGrenade(int write_pipe, Grenade *grenade) {
    while (grenade->active) {
        if (grenade->x > 0 && grenade->x < COLS - 1) {
            // Cancella la vecchia posizione
            mvprintw(grenade->y, grenade->x, " ");
            // Aggiorna la posizione in base alla direzione
            grenade->x += grenade->direction;
            // Disegna la granata
            mvprintw(grenade->y, grenade->x, "%c", GRENADE_SYMBOL);
        } else {
            grenade->active = false; // La granata esce dallo schermo
        }

        refresh();
        usleep(100000); // Ritardo per il movimento

        // Scrive nel pipe per aggiornare la posizione della granata
        write(write_pipe, grenade, sizeof(Grenade));
    }
}
