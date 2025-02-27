#include "grenade.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

void grenade(GrenadeData* g, int pipeWrite) {
    noecho();
    cbreak();
    curs_set(0);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    while (g->active) {
        // Invia la posizione della granata al padre
        write(pipeWrite, g, sizeof(GrenadeData));

        // Movimento della granata in base alla direzione
        g->x += g->dir;

        // Controllo dei limiti dello schermo
        if (g->x < 0 || g->x >= maxX) {
            g->active = 0; // Se esce dallo schermo, termina
            break;
        }

        usleep(100000); // Velocità della granata
    }

    // Ultima scrittura per segnalare che la granata non è più attiva
    write(pipeWrite, g, sizeof(GrenadeData));

    endwin();
    _exit(0);
}
