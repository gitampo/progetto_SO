#include "croc.h"
#include "entity.h"
#include "graphics.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "bullet.h"



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

void crocProcess(Entity *croc, int pipeFD) {
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    int riverStart = LINES - 27; // dove inizia il fiume, come in drawRiver()
    
    int local_proj_counter = 0;

    while (1) {
        if (croc->inGioco) {
            // Aggiorna la posizione del coccodrillo
            croc->x += croc->direction;
            if (croc->x < (startCol - CROC_WIDTH)) {
                croc->x = endCol;
            } else if (croc->x > endCol) {
                croc->x = startCol - CROC_WIDTH;
            }
        }

        // Invia la posizione aggiornata del coccodrillo al padre tramite la pipe
        write(pipeFD, croc, sizeof(Entity));
        
        // Possibilità di sparare un bullet: condizione casuale (circa 1% per iterazione)
        if (croc->inGioco && (rand() % 1000) < 10) {
            Entity bullet;
            int offsetX = (croc->direction == 1) ? CROC_WIDTH : -1;
            int bulletStartX = croc->x + offsetX;
            int bulletStartY = croc->y + (CROC_HEIGHT / 2);
            bullet.id = croc->id * 1000 + local_proj_counter++; // Un ID univoco per il bullet
            bullet.type = OBJECT_BULLET;
            createBullet(&bullet, bulletStartX, bulletStartY, croc->direction, 0); // Non è una granata
            
            pid_t pid_bullet = fork();
            if (pid_bullet == 0) {
                // Processo figlio: gestisce il movimento del bullet
                bulletProcess(&bullet, pipeFD);
                exit(EXIT_SUCCESS);  // Termina il processo figlio
            }
        }
        
        // Calcola il ritardo in base alla corsia
        int lane = (croc->y - riverStart) / CROC_HEIGHT;
        int delay;
        switch (lane) {
            case 0: delay = 500000; break;
            case 1: delay = 400000; break;
            case 2: delay = 350000; break;
            case 3: delay = 300000; break;
            case 4: delay = 250000; break;
            case 5: delay = 200000; break;
            case 6: delay = 150000; break;
            case 7: delay = 100000; break;
            default: delay = 400000; break;
        }
        usleep(delay);
    }
}
