#include "croc.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Il processo figlio esegue un ciclo infinito:
// - Muove tutti i coccodrilli
// - Scrive l'array di coccodrilli sul pipe
// - Attende un po' di tempo
void runCrocs(Crocodile crocs[], int numCrocs, int writeFD) {
    while (1) {
        // Aggiorna posizioni
        for (int i = 0; i < numCrocs; i++) {
            crocs[i].coords.x += crocs[i].direction;
            // Se esce a sinistra, ricompare a destra
            if (crocs[i].coords.x < 1) {
                crocs[i].coords.x = COLS - 2;
            }
            // Se esce a destra, ricompare a sinistra
            else if (crocs[i].coords.x > COLS - 2) {
                crocs[i].coords.x = 1;
            }
        }
        // Scrive tutto l'array sulla pipe
        // (padre leggerà con read(...) la stessa quantità di byte)
        ssize_t written = write(writeFD, crocs, sizeof(Crocodile) * numCrocs);
        if (written == -1) {
            perror("Errore write nel figlio");
            exit(1);
        }

        // Attende 400ms prima di muovere di nuovo i coccodrilli
        usleep(400000);
    }
}
