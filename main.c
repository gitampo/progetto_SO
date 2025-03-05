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
#include "scoreboard.h"

#define NUM_CROCS 1  // Puoi cambiarlo se vuoi più coccodrilli

int main(void)
{
    // 1) Inizializza ncurses (solo nel padre)
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Stampa dimensioni del terminale (opzionale, per debug)
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // mvprintw(0, 0, "DEBUG: maxX=%d, maxY=%d", maxX, maxY); refresh(); sleep(2);

    // 2) Inizializza l'area di gioco
    AreaGioco area;
    initAreaGioco(&area, maxX, maxY);

    // 3) Svuota schermo e disegna la mappa
    clear();
    disegnaAreaGioco(&area);

    // 4) Prepara la RANA
    static FrogData frog; 
    frog.id     = 1;
    // Invece di calcolare da marciapiedeBasso, forziamo una posizione ben dentro lo schermo
    // per assicurarci che la rana sia visibile:
    frog.x      = area.larghezzaSchermo / 2 + area.marciapiedeBasso.startX;
    frog.y      = area.marciapiedeBasso.startY - 3; 
    frog.width  = 5;
    frog.height = 3;
    frog.lives  = 3;

    // 5) Prepara il coccodrillo
    static CrocData croc;
    croc.id     = 2; 
    // Mettiamo il coccodrillo verso la metà del fiume più basso
    croc.x      = area.fiume[0].startX + 5;
    croc.y      = area.fiume[0].startY + 1;
    croc.oldX   = croc.x;
    croc.width  = 4;
    croc.height = 2;
    croc.alive  = 1;

    // 6) Inizializza la scoreboard 
    ScoreBoard sb;
    initScoreBoard(&sb, 3, 60);  // 3 vite, 60 secondi di tempo (se lo usi)

    // 7) Crea la pipe (figli scrivono, padre legge)
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Errore pipe");
        endwin();
        exit(1);
    }

    // 8) fork del figlio "rana"
    pid_t pidFrog = fork();
    if (pidFrog < 0) {
        perror("Errore fork rana");
        endwin();
        exit(1);
    }
    if (pidFrog == 0) {
        // Figlio: esegue frogProcess
        frogProcess(&frog, pipefd);
        exit(0);
    }

    // 9) fork del figlio "coccodrillo"
    pid_t pidCroc = fork();
    if (pidCroc < 0) {
        perror("Errore fork cocc");
        endwin();
        exit(1);
    }
    if (pidCroc == 0) {
        // Figlio: esegue crocProcess
        crocProcess(&croc, pipefd);
        exit(0);
    }

    // 10) Padre: chiude la scrittura, userà solo la lettura pipefd[0]
    close(pipefd[1]);

    // Copie locali delle strutture
    FrogData frogPos = frog;   
    CrocData crocPos = croc;

    int running = 1;  // Variabile per il loop di gioco

    // =============== LOOP PRINCIPALE ===============
    while (running) {
        // 1) Legge tutti i messaggi disponibili nella pipe
        //    (child->parent).
        ssize_t nbytes;
        do {
            char buffer[sizeof(CrocData)];
            nbytes = read(pipefd[0], buffer, sizeof(buffer));
            if (nbytes == sizeof(FrogData)) {
                // Aggiorna frogPos
                frogPos = *(FrogData*)buffer;
            }
            else if (nbytes == sizeof(CrocData)) {
                // Aggiorna crocPos
                crocPos = *(CrocData*)buffer;
            }
        } while (nbytes > 0);

        // 2) Pulisce e ridisegna l'area di gioco
        clear();
        disegnaAreaGioco(&area);

        // 3) Disegna scoreboard
        drawScoreBoard(&sb);

        // 4) Disegna la rana e il coccodrillo
        drawFrogPos(&frogPos);
        drawCrocPos(&crocPos);

        // 5) Controllo collisione rana-coccodrillo
        if (checkCollision(frogPos.x, frogPos.y, frogPos.width, frogPos.height,
                           crocPos.x, crocPos.y, crocPos.width, crocPos.height)) 
        {
            decrementLife(&sb);
            // Riporta la rana al centro in basso
            frogPos.x = area.larghezzaSchermo / 2 + area.marciapiedeBasso.startX;
            frogPos.y = area.marciapiedeBasso.startY - 3;

            if (sb.vite <= 0) {
                mvprintw(area.marciapiedeBasso.startY / 2, (area.larghezzaSchermo / 2), "GAME OVER");
                refresh();
                sleep(2);
                running = 0;  // Esce dal loop
            }
        }

        // 6) Controllo se la rana entra in una tana
        int idxTana = checkTane(&area, frogPos.x, frogPos.y,
                                frogPos.width, frogPos.height);
        if (idxTana >= 0) {
            addScore(&sb, 100);
            chiudiTana(&area.tane[idxTana]);
            // Riporta la rana al marciapiede in basso
            frogPos.x = area.larghezzaSchermo / 2 + area.marciapiedeBasso.startX;
            frogPos.y = area.marciapiedeBasso.startY - 3;
        }

        // 7) Refresh
        refresh();
        usleep(30000);
    } // fine while(running)

    // Fine: kill dei figli
    endwin();
    kill(pidFrog, SIGKILL);
    kill(pidCroc, SIGKILL);
    wait(NULL);
    wait(NULL);

    return 0;
}
