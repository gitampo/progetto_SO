#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"



void frog(Coordinates *frog, int pipe[2]) {
    while (1) {
        switch (getch()) {
            case KEY_UP:
                if (frog->y > 1) frog->y -= 1;
                break;
            case KEY_DOWN:
                if (frog->y < LINES - 2) frog->y += 1;
                break;
            case KEY_LEFT:
                if (frog->x > 1) frog->x -= 1;
                break;
            case KEY_RIGHT:
                if (frog->x < COLS - 2) frog->x += 1;
                break;
        }

        write(pipe[1], frog, sizeof(Coordinates));
        usleep(10000); // Ritardo per evitare un uso intensivo della CPU
        halfdelay(1); // Ogni chiamata a getch() aspetta fino a 100 ms
    }
}

