#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <curses.h>
#include "rana.h"

void frog(Coordinate *frog, int fileds[2]) {
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

        write(fileds[1], frog, sizeof(Coordinate));
        usleep(10000); // Ritardo per evitare un uso intensivo della CPU
        halfdelay(1); // Ogni chiamata a getch() aspetta fino a 100 ms
    }
}