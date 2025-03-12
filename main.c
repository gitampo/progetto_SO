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

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    if (can_change_color()) {
        init_color(COLOR_YELLOW, 500, 250, 0);
    }
    init_pair(3, COLOR_RED, COLOR_YELLOW);
    


    // getch() non-bloccante
    nodelay(stdscr, TRUE);

    // Creiamo la pipe (fileds[0] = read, fileds[1] = write)
    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore creazione pipe");
        endwin();
        exit(1);
    }

    // Imposto la read in modo non-bloccante
    int flags = fcntl(fileds[0], F_GETFL, 0);
    fcntl(fileds[0], F_SETFL, flags | O_NONBLOCK);

    // Inizializza la rana multi-riga
    FrogPos frog;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;

    // Inizializza i coccodrilli
    int numCrocs = 2;
    Crocodile croc1[NUM_CROCS * numCrocs];
    for (int i = 0; i < NUM_CROCS; i++) {
        for (int j = 0; j < numCrocs; j++) {
            int index = i * numCrocs + j;
            croc1[index].id = index;
            croc1[index].coords.y = 1 + i;
            croc1[index].coords.x = 1 + (i * 1) + j * 10;
            croc1[index].direction = (i % 2 == 0) ? 1 : -1;
        }
    }

    // fork per i coccodrilli
    pid_t pid_croc = fork();
    if (pid_croc == -1) {
        perror("Errore fork croc");
        endwin();
        exit(1);
    } else if (pid_croc == 0) {
        close(fileds[0]);
        runCrocs(croc1, NUM_CROCS * numCrocs, fileds[1]);
        exit(0);
    }

    // Padre: chiudo lato di scrittura
    close(fileds[1]);

    // Disegno iniziale della rana
    drawFrog(&frog);
    refresh();

    // Ciclo principale
    while (1) {
        // 1) Lettura input per la rana
        int ch = getch();
        if (ch != ERR) {
            // 1a) Cancella la vecchia posizione
            clearFrog(&frog);
            // 1b) Aggiorna coordinate
            moveFrog(&frog, ch);

            // Se vuoi limitare il movimento orizzontale al marciapiede centrato:
            int startCol = (COLS - PAVEMENT_WIDTH) / 2;  // colonna di inizio
            int endCol   = startCol + PAVEMENT_WIDTH;    // colonna di fine (esclusa)
            if (frog.x < startCol) {
                frog.x = startCol;
            }
            int maxX = endCol - FROG_WIDTH + 2;
            if (frog.x > maxX) {
                frog.x = maxX;
            }
        }

        // 2) Leggiamo la posizione dei coccodrilli dal figlio
        ssize_t bytesRead = read(fileds[0], croc1, sizeof(croc1));
        (void)bytesRead; // per evitare warning "unused variable"

        // 3) Controllo collisioni
        // for (int i = 0; i < NUM_CROCS; i++) {
        //     int cx = croc1[i].coords.x;
        //     int cy = croc1[i].coords.y;
        //     if (cx >= frog.x && cx < frog.x + FROG_WIDTH &&
        //         cy >= frog.y && cy < frog.y + FROG_HEIGHT) 
        //     {
        //         // Scontro: resetto la rana
        //         clearFrog(&frog);
        //         frog.y = LINES - FROG_HEIGHT;
        //         frog.x = (COLS - FROG_WIDTH) / 2;
        //     }
        // }

        // 4) Ridisegno schermo
        clear();
        drawRiver();     // fiume
        drawPavement();     // marciapiede in basso 
        drawFrog(&frog);
        for (int i = 0; i < NUM_CROCS * numCrocs; i++) {
            mvaddch(croc1[i].coords.y, croc1[i].coords.x, SPRITE_CROC);
        }
        refresh();

        // 5) Pausa ~70ms
        usleep(70000);
    }

    // Codice non raggiunto con while(1):
    endwin();
    kill(pid_croc, SIGKILL);
    wait(NULL);
    return 0;
}
