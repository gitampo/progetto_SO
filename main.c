#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <fcntl.h> // Per impostare O_NONBLOCK
#include "graphics.h"
#include "frog.h"
#include "croc.h"

int main() {
    // Inizializzazione ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Rendiamo getch() non-bloccante (così il programma continua anche senza input)
    nodelay(stdscr, TRUE);

    // Creiamo la pipe (fileds[0] per leggere, fileds[1] per scrivere)
    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore creazione pipe");
        endwin();
        exit(1);
    }

    // Impostiamo la lettura dal pipe in modo non-bloccante
    // (così il padre non si blocca in read() se il figlio non ha ancora scritto)
    int flags = fcntl(fileds[0], F_GETFL, 0);
    fcntl(fileds[0], F_SETFL, flags | O_NONBLOCK);

    // Inizializza la rana
    Coordinates frog1;
    frog1.id = 1;
    frog1.y = LINES - 2;
    frog1.x = COLS / 2;

    // Inizializza un array di coccodrilli
    Crocodile croc1[NUM_CROCS];
    for (int i = 0; i < NUM_CROCS; i++) {
        croc1[i].id = i + 2;  // ID univoco
        croc1[i].coords.y = 5 + i;        // sparsi verticalmente
        croc1[i].coords.x = 5 + (i * 5);  // e orizzontalmente
        // alterniamo la direzione
        croc1[i].direction = (i % 2 == 0) ? 1 : -1;
    }

    // Creiamo un processo figlio per gestire tutti i coccodrilli
    pid_t pid_croc = fork();
    if (pid_croc == -1) {
        perror("Errore fork croc");
        endwin();
        exit(1);
    } else if (pid_croc == 0) {
        // Figlio: chiude il lato di lettura
        close(fileds[0]);
        // Avvia la gestione dei coccodrilli
        runCrocs(croc1, NUM_CROCS, fileds[1]);
        // In teoria non usciamo mai, ma se volessi chiudere:
        exit(0);
    }

    // Padre: chiude il lato di scrittura
    close(fileds[1]);

    // Ciclo principale del gioco
    while (1) {
        // 1) Leggiamo input utente per la rana
        int ch = getch(); // se non c'è input, restituisce ERR
        if (ch != ERR) {
            moveFrog(&frog1, ch);
        }

        // 2) Leggiamo le nuove posizioni dei coccodrilli dal figlio (non bloccante)
        ssize_t bytesRead = read(fileds[0], croc1, sizeof(croc1));
        // Se bytesRead == -1 e errno == EAGAIN o EWOULDBLOCK => nessun dato pronto
        // Se bytesRead == sizeof(croc1), allora abbiamo un array completo
        // Altrimenti potrebbe esserci un errore o fine del figlio
        // Per semplicità, ignoriamo i casi parziali e proseguiamo
        // (ma in un progetto più robusto gestiremmo i vari possibili casi).
        
        // 3) Controllo collisioni (frog1 vs coccodrilli)
        for (int i = 0; i < NUM_CROCS; i++) {
            if (frog1.y == croc1[i].coords.y && frog1.x == croc1[i].coords.x) {
                // Scontro: rimettiamo la rana in basso
                frog1.y = LINES - 2;
                frog1.x = COLS / 2;
            }
        }

        // 4) Puliamo lo schermo e ridisegniamo
        clear();

        // Disegniamo la rana
        mvaddch(frog1.y, frog1.x, SPRITE_FROG);

        // Disegniamo i coccodrilli
        for (int i = 0; i < NUM_CROCS; i++) {
            mvaddch(croc1[i].coords.y, croc1[i].coords.x, SPRITE_CROC);
        }

        // Aggiorna lo schermo
        refresh();

        // 5) Pausa di ~70ms (14 FPS circa)
        usleep(70000);
    }

    // (Se prevedi di uscire dal gioco, termina figlio e chiudi ncurses)
    endwin();
    kill(pid_croc, SIGKILL);
    wait(NULL);
    return 0;
}
