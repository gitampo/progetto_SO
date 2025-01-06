#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <curses.h>
#include <time.h>
#include "croc.h"
#include "frog.h"

void croc(Coordinate *croc1, int fileds[2]){

    while(1){

    // Gestisce il looping ai bordi della finestra
    
    if (croc1->x >= COLS - 2) {
        croc1->x = COLS - 20; // Ricomincia dal bordo sinistro (è sbagliato stavo provando)
        croc1->x += 1;
    } else if (croc1->x <= COLS -21) {
        croc1->x = COLS- 2; // Ricomincia dal bordo destro (è sbagliato stavo provando)
        croc1->x -= 1;
    }
}

    write(fileds[1], croc1, sizeof(Coordinate));
    usleep(10000);
}
