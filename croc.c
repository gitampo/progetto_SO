#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <curses.h>
#include <time.h>
#include "croc.h"
#include "frog.h"

void croc(Coordinate *croc, int fileds[2]){

int counter = 0;

    while(1){

    if (counter % 5 == 0) // Muove il coccodrillo ogni 5 cicli
        croc->x += 1;
    
    // Controlla se è uscito dallo schermo
    if (croc->x > 177) {
        croc->x = -11;  // Riappare sul lato opposto
    } else if (croc->x < -11) {
        croc->x = 177;  // Riappare sul lato opposto
    }

    write(fileds[1], croc, sizeof(Coordinate));
    refresh();
    
    counter++;
    usleep(50000);

    }

    
}

//larghezza = 178, altezza 14