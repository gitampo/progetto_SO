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

    // 4) Prepara la RANA da passare al figlio
    //    (La struct iniziale è su stack, la passiamo per value a fork,
    //     ma meglio se la mettiamo su variabile globale o static. Esempio semplificato.)
    static FrogData frog; 
    frog.id     = 1;
    frog.x      = maxX/2;
    frog.y      = area.marciapiedeBasso.startY - 3; // 3 righe di ASCII
    frog.width  = 5;
    frog.height = 3;
    frog.lives  = 3;

    // 5) Prepara il Coccodrillo
    static CrocData croc;
    croc.id     = 2; 
    croc.x      = 5; 
    croc.y      = area.fiume[0].startY + 1;
    croc.oldX   = croc.x;
    croc.width  = 4;
    croc.height = 2;
    croc.alive  = 1;

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
        // figlio rana
        frogProcess(&frog, pipefd);
        // se esce dal while, break
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
        // figlio cocc
        crocProcess(&croc, pipefd);
        exit(0);
    }

    // 9) Processo padre chiude la scrittura
    close(pipefd[1]);

    // Variabili locali per salvare la posizione aggiornata
    // da ogni messaggio in arrivo
    FrogData frogPos = frog;    // copia
    CrocData crocPos = croc;    // copia

    // ESEMPIO: se vuoi più coccodrilli, puoi avere un array di CrocData

    // 10) Main loop: il padre legge dalla pipe e disegna
    int running = 1;
    while (running) {
        // Leggo un messaggio generico (dimensione max tra FrogData e CrocData)
        // Qui assumo che i figli scrivano la stessa dimensione di struct
        // e che il "id" distingua chi è chi
        char buffer[sizeof(CrocData)]; 
        int n = read(pipefd[0], buffer, sizeof(CrocData));
        if (n <= 0) {
            // Nessun dato => facciamo un piccolo sleep e ridisegniamo
            usleep(30000);
        } else {
            // Interpretiamo i primi 4 byte come "id"
            int *pId = (int*) buffer;
            int who = *pId;

            if (who == 1) {
                // è la rana
                FrogData *fptr = (FrogData*) buffer;
                frogPos = *fptr;
            } else if (who == 2) {
                // è il coccodrillo
                CrocData *cptr = (CrocData*) buffer;
                crocPos = *cptr;
            }
            // Se avessi più coccodrilli con id 2..N,
            // puoi avere un array e fare cPos[id-2] = *cptr, ecc.
        }

        // 11) Ridisegna l’area + personaggi
        clear();
        disegnaAreaGioco(&area);

        // Disegna la rana
        drawFrogPos(&frogPos);

        // Disegna cocc
        drawCrocPos(&crocPos);

        // Se vuoi collisioni
        if (checkCollision(frogPos.x, frogPos.y, frogPos.width, frogPos.height,
                           crocPos.x, crocPos.y, crocPos.width, crocPos.height)) {
            // collisione => riduci vite, reset
            frogPos.lives--;
            frogPos.x = maxX/2;
            frogPos.y = area.marciapiedeBasso.startY - 3;
            if (frogPos.lives <= 0) {
                mvprintw(maxY/2, (maxX/2)-5, "GAME OVER");
                refresh();
                sleep(2);
                running=0;
            }
        }

        // check tana
        int idxTana = checkTane(&area, frogPos.x, frogPos.y, frogPos.width, frogPos.height);
        if(idxTana >= 0){
            // la rana e' in tana
            chiudiTana(&area.tane[idxTana]);
            // rimetti la rana in basso
            frogPos.x = maxX/2;
            frogPos.y = area.marciapiedeBasso.startY - 3;
        }

        refresh();
        usleep(30000);
    }

    // 12) Fine: kill figli e aspetta
    endwin();
    kill(pidFrog, SIGKILL);
    kill(pidCroc, SIGKILL);
    wait(NULL);
    wait(NULL);

    return 0;
}
