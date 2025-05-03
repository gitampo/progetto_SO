#include "croc.h"
#include "entity.h"
#include "graphics.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bullet.h"


// Disegna un coccodrillo
void drawCrocodiles(const Entity crocs[], int totalCrocs ) {
    attron(COLOR_PAIR(4));
    for (int i = 0; i < totalCrocs; i++) {
        if (!crocs[i].inGioco) continue;
        for (int row = 0; row < CROC_HEIGHT; row++) {
            for (int col = 0; col < CROC_WIDTH; col++) {
                if (crocs[i].x + col >= ((COLS - PAVEMENT_WIDTH) / 2) &&
                    crocs[i].x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
                    mvprintw(crocs[i].y + row, crocs[i].x + col, "%c", SPRITE_CROC);
            }
        }
    }
    attroff(COLOR_PAIR(4));
}


void creaCrocodiles(Entity crocs[], int startCol, int endCol, int riverStartRow) {
    int index = 0;
    for (int i = 0; i < LANES; i++) {
        int dir = (i % 2 == 0) ? 1 : -1;  // corsia pari: da sinistra a destra; dispari: da destra a sinistra
        for (int j = 0; j < CROCS_PER_LANE; j++) {
            crocs[index].id = index; // Rana ha id 1, Coccodrilli da 2
            crocs[index].type = OBJECT_CROCODILE;
            crocs[index].y = riverStartRow + i * CROC_HEIGHT;
            crocs[index].direction = dir;
            if (dir == 1)
                crocs[index].x = startCol + j * 30; //distanza coccodrilli per riga
            else
                crocs[index].x = endCol - CROC_WIDTH - j * 30;
            crocs[index].initX = crocs[index].x;
            crocs[index].inGioco = 1; // Imposta inGioco a 1
            index++;
        }
    }
}

void crocProcess(Entity *croc, int fileds, int riverStart) {
    srand(getpid()); // Inizializza il seme del generatore casuale
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    
    int local_proj_counter = 0;

     // Calcola il ritardo in base alla corsia
    int lane = (croc->y - riverStart) / CROC_HEIGHT;
   
    switch(lane) {
        case 0: croc->speed = 1; break;  
        case 1: croc->speed = 2; break;
        case 2: croc->speed = 3; break;
        case 3: croc->speed = 1; break;
        case 4: croc->speed = 2; break;  
        default: croc->speed = 2; break; // Default se ci sono più corsie
    }

    while (1) {
            // Aggiorna la posizione del coccodrillo
        croc->x += croc->direction * croc->speed; 
        if (croc->x < (startCol - CROC_WIDTH)) {
            croc->x = endCol;
        } else if (croc->x > endCol) {
            croc->x = startCol - CROC_WIDTH;
        }

        // Invia la posizione aggiornata del coccodrillo al padre tramite la pipe
        write(fileds, croc, sizeof(Entity));
        
        // Possibilità di sparare un bullet: condizione casuale (circa 1% per iterazione)
        if ((croc->direction == 1 && croc->x + CROC_WIDTH < endCol ||
            croc->direction == -1 && croc->x > startCol) && rand() % 1000 < 10) {
            Entity bullet;
            int offsetX = (croc->direction == 1) ? CROC_WIDTH : -1;
            int bulletStartX = croc->x + offsetX;
            int bulletStartY = croc->y + (CROC_HEIGHT / 2);
            bullet.type = CREATE_BULLET;
            bullet.x = bulletStartX;
            bullet.y = bulletStartY;
            bullet.direction = croc->direction;
            bullet.inGioco = 1;
            bullet.speed = 2; // Velocità del proiettile
            write(fileds, &bullet, sizeof(Entity)); // Invia il bullet al processo padre
        }

        usleep(100000);

    }
}
