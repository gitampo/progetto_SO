#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"
#include  "croc.h"

void croc(Coordinates *croc, int fileds[2]) {

    while (1) {
            for (int i = 0; i < NUM_CROCS; i++) {
                croc[i].x += 1;
                if (croc[i].x >= COLS) {
                    croc[i].x = 0;
                }
                
            }
            usleep(50000 * croc->id); 
            write(fileds[1], croc, sizeof(Coordinates));
    }
}

