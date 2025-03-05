#include "croc.h"
#include <unistd.h> // usleep, write
#include <ncurses.h> // se vuoi debug (mvprintw)

void crocProcess(CrocData* croc, int pipefd[2])
{
    // Il figlio coccodrillo non legge, quindi chiude pipefd[0]
    close(pipefd[0]);

    int counter = 0;
    while (1) {
        // Semplice logica: muovi a destra di 1 ogni 5 cicli
        if (counter % 5 == 0) {
            croc->x++;
        }
        // Se usciamo troppo a destra (x>100?), riportiamolo a sinistra
        if (croc->x > 100) {
            croc->x = -croc->width;
        }

        // Scrive la struct al padre
        write(pipefd[1], croc, sizeof(*croc));

        counter++;
        usleep(60000);
    }
}
