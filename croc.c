#include "croc.h"
#include <unistd.h> // usleep, write
#include <ncurses.h>

/*
 * Funzione eseguita dal FIGLIO "coccodrillo":
 *  - chiude lettura pipe
 *  - muove il coccodrillo e invia coordinate al padre
 */
void crocProcess(CrocData* croc, int pipefd[2])
{
    close(pipefd[0]); // chiudi lettura

    int counter=0;
    while (1) {
        // Esempio di logica di movimento
        if (counter % 5 == 0) {
            croc->x++;
        }
        if (croc->x > 100) { // se va oltre 100, torna a sinistra
            croc->x = -croc->width;
        }

        // scrivo la struct
        write(pipefd[1], croc, sizeof(*croc));

        counter++;
        usleep(60000);
    }
}
