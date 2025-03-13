#include "graphics.h"
#include "croc.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void runCrocs(Entity crocs[], int numCrocs, int pipeFD) {
    // Calcola i limiti orizzontali del fiume (l'area del fiume, centrata)
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol   = startCol + PAVEMENT_WIDTH;

    while (1) {
        for (int i = 0; i < numCrocs; i++) {
            // Aggiorna la coordinata x in base alla direzione
            crocs[i].x += (crocs[i].type == OBJECT_CROCODILE) ? 1 : 0; // per semplicità, supponiamo che la direzione sia incorporata nella logica
            // Se il coccodrillo esce troppo a sinistra o a destra, respawn
            if (crocs[i].x < (startCol - COCC_WIDTH) || crocs[i].x > endCol) {
                // Respawn alla posizione iniziale, che può essere memorizzata in un campo separato se necessario
                // Per ora, ipotizziamo di respawnare in una posizione fissa, ad es. all'inizio della corsia
                // (Potresti voler salvare l'iniziale in un campo; qui semplifichiamo)
                // Ad esempio, riposiziona in base alla corsia (calcolabile dal valore di y)
                // Se y è pari a startRow + lane * COCC_HEIGHT, respawn in x = startCol (oppure a destra se direzione negativa)
                // Per questo esempio, lasciamo la logica originale:
                crocs[i].x = startCol + (i % 2) * 10; 
            }
        }
        ssize_t written = write(pipeFD, crocs, sizeof(Entity) * numCrocs);
        if (written == -1) {
            perror("Errore write nel figlio (coccodrilli)");
            exit(1);
        }
        usleep(400000);
    }
}
