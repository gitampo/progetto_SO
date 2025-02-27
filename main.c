#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>

#include "config.h"
#include "graphics.h"
#include "gameLogic.h"
#include "frog.h"
#include "croc.h"
#include "grenade.h"

// 1) Includi scoreboard
#include "scoreboard.h"

#define NUM_CROCS 1  // Per esempio 1 coccodrillo

int main(void)
{
    // 1) Inizializza ncurses (solo nel padre)
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // 2) Inizializza l'area di gioco
    AreaGioco area;
    initAreaGioco(&area, maxX, maxY);

    // 3) Svuota schermo e disegna la mappa
    clear();
    disegnaAreaGioco(&area);

    // 4) Prepara la RANA 
    static FrogData frog; 
    frog.id     = 1;
    frog.x = area.larghezzaSchermo / 2 + area.marciapiedeBasso.startX;
    frog.y = area.marciapiedeBasso.startY - 3;
    frog.width  = 5;
    frog.height = 3;
    frog.lives  = 3;

    // 5) Prepara il Coccodrillo
    static CrocData croc;
    croc.id     = 2; 
    croc.x = area.fiume[0].startX + 5;
    croc.y = area.fiume[0].startY + 1;
    croc.oldX   = croc.x;
    croc.width  = 4;
    croc.height = 2;
    croc.alive  = 1;

    // 5bis) Creiamo e inizializziamo la scoreboard 
    ScoreBoard sb;
    initScoreBoard(&sb, 3, 60);

    // 6) Crea la pipe
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Errore pipe");
        endwin();
        exit(1);
    }

    // 7) fork del figlio "rana"
    pid_t pidFrog = fork();
    if (pidFrog < 0) {
        perror("Errore fork rana");
        endwin();
        exit(1);
    }
    if (pidFrog == 0) {
        frogProcess(&frog, pipefd);
        exit(0);
    }

    // 8) fork del figlio "coccodrillo"
    pid_t pidCroc = fork();
    if (pidCroc < 0) {
        perror("Errore fork cocc");
        endwin();
        exit(1);
    }
    if (pidCroc == 0) {
        crocProcess(&croc, pipefd);
        exit(0);
    }

    // 9) Processo padre chiude la scrittura
    close(pipefd[1]);

    // Copie locali delle strutture 
    FrogData frogPos = frog;   
    CrocData crocPos = croc;   
    GrenadeData grenadePos;  // Per gestire le granate

    int running = 1;
    while (running) {
        // Legge un messaggio dalla pipe (massima dimensione tra le strutture)
        char buffer[sizeof(GrenadeData)];
        int n = read(pipefd[0], buffer, sizeof(GrenadeData));
        if (n > 0) {
            int *pId = (int*) buffer;
            int who = *pId;
            
            if (who == 1) {  // RANA
                FrogData *fptr = (FrogData*) buffer;
                frogPos = *fptr;
            } 
            else if (who == 2) {  // COCCODRILLO
                CrocData *cptr = (CrocData*) buffer;
                crocPos = *cptr;
            } 
            else if (who == 3) {  // GRANATA
                GrenadeData *gptr = (GrenadeData*) buffer;
                grenadePos = *gptr;
            
                // Controllo collisione granata-coccodrillo
                if (checkCollision(grenadePos.x, grenadePos.y, grenadePos.width, grenadePos.height,
                                   crocPos.x, crocPos.y, crocPos.width, crocPos.height)) {
                    crocPos.alive = 0;  // Il coccodrillo viene eliminato
                    grenadePos.active = 0;  // La granata sparisce
                }
            }
            
        }

        // 1) Pulisce e ridisegna l'area di gioco
        clear();
        disegnaAreaGioco(&area);

        // 2) Disegna scoreboard
        drawScoreBoard(&sb);

        // 3) Disegna rana, coccodrillo e granata
        drawFrogPos(&frogPos);
        drawCrocPos(&crocPos);
        drawGrenadePos(&grenadePos);  // Disegna la granata

        // 4) Controlla collisione rana-coccodrillo
        if (checkCollision(frogPos.x, frogPos.y, frogPos.width, frogPos.height,
                           crocPos.x, crocPos.y, crocPos.width, crocPos.height)) {
            decrementLife(&sb);
            frogPos.x = maxX/2;
            frogPos.y = area.marciapiedeBasso.startY - 3;

            if (sb.vite <= 0) {
                mvprintw(maxY/2, (maxX/2)-5, "GAME OVER");
                refresh();
                sleep(2);
                running = 0;
            }
        }

        // 5) Controllo collisione granata-coccodrillo
        if (checkCollision(grenadePos.x, grenadePos.y, grenadePos.width, grenadePos.height,
                           crocPos.x, crocPos.y, crocPos.width, crocPos.height)) {
            crocPos.alive = 0;  // Il coccodrillo viene eliminato
            grenadePos.active = 0;  // La granata sparisce
        }

        // Controllo se la rana entra in una tana
     int idxTana = checkTane(&area, frogPos.x, frogPos.y, 
    frogPos.width, frogPos.height);

    if(idxTana >= 0){
        addScore(&sb, 100);
        chiudiTana(&area.tane[idxTana]);
    
        // Nuova posizione della rana nel marciapiede inferiore
        frogPos.x = area.marciapiedeBasso.startX + area.larghezzaSchermo / 2;
        frogPos.y = area.marciapiedeBasso.startY + 1;
    
        // Imposta ID speciale per segnalare aggiornamento della posizione
        frogPos.id = -1;
    
        // **Invia immediatamente la nuova posizione al processo rana**
        write(pipefd[1], &frogPos, sizeof(FrogData));
    
        // **Aspetta un momento per sincronizzare i processi**
        usleep(100000);
    }
    
    
    
        
        // 7) refresh finale
        refresh();
        usleep(30000);
    }

    // Fine: kill dei figli
    endwin();
    kill(pidFrog, SIGKILL);
    kill(pidCroc, SIGKILL);
    wait(NULL);
    wait(NULL);

    return 0;
}
