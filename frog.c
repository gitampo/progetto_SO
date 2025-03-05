#include "frog.h"
#include <ncurses.h>
#include <unistd.h>    // write(), usleep()
#include <stdlib.h>    // exit()
#include <signal.h>
#include <sys/wait.h>

static void handle_sigchld(int sig) {
    (void)sig;  // Silenzia warning "unused parameter"

    while (waitpid(-1, NULL, WNOHANG) > 0) {
        // consume all dead children
    }
}

void frogProcess(FrogData* frog, int pipefd[2]) {
    // Figlio: chiude l'estremità di lettura (pipefd[0]),
    // perché scriverà solo su pipefd[1].
    close(pipefd[0]);

    // Evitare zombie se ci sono ulteriori fork
    signal(SIGCHLD, handle_sigchld);

    // Ciclo principale: prende input da tastiera e invia nuove posizioni al padre
    while (1) {
        int ch = getch();  // Raccoglie tasto
        switch (ch) {
            case KEY_UP:    frog->y -= frog->height; break;
            case KEY_DOWN:  frog->y += frog->height; break;
            case KEY_LEFT:  frog->x -= frog->width;  break;
            case KEY_RIGHT: frog->x += frog->width;  break;
            default:
                // Nessun movimento
                break;
        }

        // Manda al padre l'aggiornamento
        write(pipefd[1], frog, sizeof(*frog));
        usleep(50000);
    }
}
