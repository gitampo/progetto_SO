#include "croc.h"
#include "entity.h"
#include "graphics.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
    
    while (1) {
        if (croc->inGioco) {
            // Aggiorna la posizione in base alla direzione
            croc->x += croc->direction;
            // Controlla i limiti orizzontali e respawna se necessario
            if (croc->x < (startCol - CROC_WIDTH)) {
                croc->x = endCol;
            } else if (croc->x > endCol) {
                croc->x = startCol - CROC_WIDTH;
            }
        }
        // Invia l'aggiornamento sulla pipe
        write(pipeFD, croc, sizeof(Entity));
        
        // Calcola la corsia in base alla coordinata Y
        int lane = (croc->y - riverStart) / CROC_HEIGHT;
        int delay;
        switch (lane) {
            case 0: delay = 500000; break; // corsia 0, delay 500ms
            case 1: delay = 400000; break; // corsia 1, delay 400ms
            case 2: delay = 350000; break; // corsia 2, delay 350ms
            case 3: delay = 300000; break; // corsia 3, delay 300ms
            case 4: delay = 250000; break; // corsia 4, delay 250ms
            case 5: delay = 200000; break; // corsia 5, delay 200ms
            case 6: delay = 150000; break; // corsia 6, delay 150ms
            case 7: delay = 100000; break; // corsia 7, delay 100ms
            default: delay = 400000; break;
        }
        usleep(delay);
    }
}

