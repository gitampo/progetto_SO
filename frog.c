#include "frog.h"
#include "grenade.h"
#include <ncurses.h>   // getch()
#include <unistd.h>    // write(), usleep()
#include <signal.h>    // SIGKILL se serve
#include <stdlib.h>    // fork(), exit()
#include "graphics.h"  // checkTane()
#include "gameLogic.h" //addScore()

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
       
                        // Controllo se la rana entra in una tana
     int idxTana = checkTane(&area, frog->x, frog->y, 
        frog->width, frog->height);
    
        if(idxTana >= 0){
            addScore(&sb, 100);
            chiudiTana(&area.tane[idxTana]);
        
            // Nuova posizione della rana nel marciapiede inferiore
            frog->x = area.marciapiedeBasso.startX + area.larghezzaSchermo / 2;
            frog->y = area.marciapiedeBasso.startY + 1;
        
            // Imposta ID speciale per segnalare aggiornamento della posizione
            frog->id = -1;
        
            printf("[DEBUG] Invio posizione con id=%d, x=%d, y=%d\n", frog->id, frog->x, frog->y);
            fflush(stdout);
         
        }

      
        // **Ora legge il tasto normalmente**
        int ch = getch();
        switch (ch) {
            case KEY_UP:    frog->y -= frog->height; break;
            case KEY_DOWN:  frog->y += frog->height; break;
            case KEY_LEFT:  frog->x -= frog->width; break;
            case KEY_RIGHT: frog->x += frog->width; break;
           
                break;
            default:
                break;
        }

        
        // **Scrivi la struct aggiornata nella pipe principale**
        write(pipefd[1], frog, sizeof(*frog));
        usleep(50000);
    }
}
