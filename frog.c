#include "frog.h"
#include "grenade.h"
#include <ncurses.h>   // getch()
#include <unistd.h>    // write(), usleep()
#include <signal.h>    // SIGKILL se serve
#include <stdlib.h>    // fork(), exit()

void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

#include <fcntl.h>  // **Aggiunto per la pipe non bloccante**

void frogProcess(FrogData* frog, int pipefd[2]) {
    // Chiude lettura della pipe principale
    close(pipefd[0]);

    // Ignora SIGCHLD per evitare zombie process
    signal(SIGCHLD, handle_sigchld);

    // **Imposta la pipe in modalità non bloccante**
    fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

    // **Ciclo principale**
    while (1) {
        // **Controlla se ci sono aggiornamenti nella pipe (senza bloccare)**
        FrogData tempFrog;
        int n = read(pipefd[1], &tempFrog, sizeof(FrogData));

        if (n > 0 && tempFrog.id == -1) {  // ID speciale per aggiornamento posizione
            *frog = tempFrog;  // **Aggiorna la posizione della rana**

           
        }

        // **Ora legge il tasto normalmente**
        int ch = getch();
        switch (ch) {
            case KEY_UP:    frog->y -= frog->height; break;
            case KEY_DOWN:  frog->y += frog->height; break;
            case KEY_LEFT:  frog->x -= frog->width; break;
            case KEY_RIGHT: frog->x += frog->width; break;
            case ' ':
                if (fork() == 0) {
                    setsid();
                    GrenadeData g;
                    g.id = 3;
                    g.active = 1;
                    g.x = frog->x - 1;
                    g.y = frog->y;
                    g.width = 1;
                    g.height = 1;
                    g.dir = -1;
                    grenade(&g, pipefd[1]);
                    _exit(0);
                }
                if (fork() == 0) {
                    setsid();
                    GrenadeData g;
                    g.id = 3;
                    g.active = 1;
                    g.x = frog->x + frog->width;
                    g.y = frog->y;
                    g.width = 1;
                    g.height = 1;
                    g.dir = 1;
                    grenade(&g, pipefd[1]);
                    _exit(0);
                }
                break;
            default:
                break;
        }

        

        // **Scrivi la struct aggiornata nella pipe principale**
        write(pipefd[1], frog, sizeof(*frog));
        usleep(50000);
    }
}
