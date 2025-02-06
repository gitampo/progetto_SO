#include "frog.h"
#include <ncurses.h>   // getch()
#include <unistd.h>    // write(), usleep()
#include <signal.h>    // SIGKILL se serve

/*
 * Funzione eseguita dal FIGLIO "rana":
 *  - Chiude lettura pipe
 *  - In un while(1), fa getch(), aggiorna (x,y), e scrive su pipe
 *
 *  Attenzione: spesso getch() non funziona nei figli,
 *  perché solo il padre ha initscr(). 
 *  Questo esempio è didattico; potrebbe non funzionare su alcuni terminali.
 */
void frogProcess(FrogData* frog, int pipefd[2])
{
    // Chiude lettura
    close(pipefd[0]);

    // Ciclo infinito
    while(1) {
        // Leggi tasto
        int ch = getch(); 
        switch(ch) {
            case KEY_UP:    frog->y--; break;
            case KEY_DOWN:  frog->y++; break;
            case KEY_LEFT:  frog->x--; break;
            case KEY_RIGHT: frog->x++; break;
            default:
                // ignora
                break;
        }

        // Scrivi la struct aggiornata
        write(pipefd[1], frog, sizeof(*frog));
        usleep(50000);
    }
}
