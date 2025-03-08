#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"
#include "croc.h"

void croc(Crocodile *croc, int fileds[2]) {
    while (1) {
        // Muove il coccodrillo orizzontalmente
        croc->coords.x += 1;
        if (croc->coords.x >= COLS) {
            croc->coords.x = 0; // Riappare dal lato opposto
        }
        if (croc->id == 2 && croc->id == 5) {
            croc->coords.x -= 1;
            if (croc->coords.x <= COLS) {
                croc->coords.x = 10; // Riappare dal lato opposto
            }
        }
        // Scrive le nuove coordinate nel pipe
        write(fileds[1], &croc->coords, sizeof(Coordinates));
        // Attende un po' di tempo prima di muoversi di nuovo
        usleep(500000); // 500 millisecondi
    }
}

