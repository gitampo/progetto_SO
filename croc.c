#include "croc.h"
#include "entity.h"
#include "graphics.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void creaCrocodiles(Entity crocs[], int startCol, int endCol, int riverStartRow) {
    int index = 0;
    for (int lane = 0; lane < LANES; lane++) {
        int dir = (lane % 2 == 0) ? 1 : -1;
        for (int j = 0; j < CROCS_PER_LANE; j++) {
            crocs[index].id = index; // Rana ha id 1, coccodrilli da 2 in poi
            crocs[index].type = OBJECT_CROCODILE;
            crocs[index].y = riverStartRow + lane * COCC_HEIGHT;
            crocs[index].direction = dir;
            if (dir == 1)
                crocs[index].x = startCol + j * 10;
            else
                crocs[index].x = endCol - COCC_WIDTH - j * 10;
            crocs[index].initX = crocs[index].x;
            index++;
        }
    }
}

void crocProcess(Entity *croc, int pipeFD) {
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    while (1) {
       if (croc->inGioco) {
            croc->x += croc->direction * 2;
            if (croc->x < (startCol - COCC_WIDTH) || croc->x > endCol) {
                // Il coccodrillo è "morto": nascondilo e imposta inGioco a 0
                croc->x = FUORI;
                croc->inGioco = 0;
                write(pipeFD, croc, sizeof(Entity));
                 //Delay di respawn
                 usleep(1000000); // attende 1 secondo prima di respawnare
                 croc->x = croc->initX;
                 croc->inGioco = 1;
            }
        }
        write(pipeFD, croc, sizeof(Entity));
        usleep(400000);
    }
}