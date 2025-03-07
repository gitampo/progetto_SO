#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"
#include "frog.h"

void frog(Coordinates *frog, int fileds[2]) {
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
        write(fileds[1], frog, sizeof(Coordinates));
    }
}

