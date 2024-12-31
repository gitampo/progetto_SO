#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <curses.h>

const char *frog[] = {
    "         @..@",
    "        (----)",
    "       ( >__< )",
    "       ^^ ~~ ^^"
};

typedef struct {
    int x, y; // Posizione della rana
} Coordinate;

void drawFrog(Coordinate *frogPosition) {
    clear();
    for (int i = 0; i < 4; i++) {
        mvprintw(frogPosition->y + i, frogPosition->x, frog[i]); // Disegna ogni riga dello sprite
    }
    refresh();
}

void handleFrogMovement(Coordinate *frog, int fileds[2]) {
    while (1) {
        switch (getch()) {
            case KEY_UP:
                if (frog->y > 1) frog->y -= 1;
                break;
            case KEY_DOWN:
                if (frog->y < LINES - 5) frog->y += 1; // 5 per lasciare spazio alla rana
                break;
            case KEY_LEFT:
                if (frog->x > 1) frog->x -= 1;
                break;
            case KEY_RIGHT:
                if (frog->x < COLS - 12) frog->x += 1; // 12 per la larghezza dello sprite
                break;
        }

        drawFrog(frog); // Aggiorna la rana sullo schermo
        write(fileds[1], frog, sizeof(Coordinate)); // Comunica la posizione tramite pipe
        usleep(10000); // Ritardo per evitare un uso intensivo della CPU
    }
}

