#include "frog.h"
#include <ncurses.h>

// Muove la rana in base al tasto premuto (freccia su/giu/sinistra/destra).
void moveFrog(Coordinates *f, int inputKey) {
    switch (inputKey) {
        case KEY_UP:
            if (f->y > 1) {
                f->y--;
            }
            break;
        case KEY_DOWN:
            if (f->y < LINES - 2) {
                f->y++;
            }
            break;
        case KEY_LEFT:
            if (f->x > 1) {
                f->x--;
            }
            break;
        case KEY_RIGHT:
            if (f->x < COLS - 2) {
                f->x++;
            }
            break;
        default:
            // Nessun movimento se il tasto non è una freccia
            break;
    }
}
