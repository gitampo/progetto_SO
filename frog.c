#include "graphics.h"
#include "frog.h"
#include <ncurses.h>
#include <unistd.h>
#include "bullet.h"
#include <stdlib.h>  // Per dichiarare exit() e EXIT_SUCCESS


extern int taneOccupate[NUM_TANE];  // Permette a frog.c di usare la variabile
 
void drawFrog(const Entity *frog) { 
    attron(COLOR_PAIR(1));
    mvprintw(frog->y,     frog->x, SYMBOL_FROG_1); 
    mvprintw(frog->y + 1, frog->x, SYMBOL_FROG_2); 
    mvprintw(frog->y + 2, frog->x, SYMBOL_FROG_3); 
    attroff(COLOR_PAIR(1));
} 
 
void clearFrog(const Entity *frog) { 
    for (int i = 0; i < FROG_HEIGHT; i++) { 
        mvprintw(frog->y + i, frog->x + i, "        "); // 8 spazi 
    } 
} 
 
void frogProcess(Entity *frog, int fileds[2], int toFrog[2]) {
    frog->type = OBJECT_FROG;
    int validX_min = (COLS - PAVEMENT_WIDTH) / 2;
    int validX_max = validX_min + PAVEMENT_WIDTH - FROG_WIDTH;
    int validY_min = LINES - 33;              
    int validY_max = LINES - FROG_HEIGHT;       
    Entity temp;
    
    while (1) {
        Entity payload = *frog;  // sempre inizializza il payload aggiornato

        if (read(toFrog[0], &temp, sizeof(Entity)) > 0) {
            *frog = temp;
            payload = *frog;
        }

        int ch = getch();
        if (ch != ERR) {
            switch(ch) {
                case KEY_UP:
                    if (frog->y - FROG_HEIGHT >= validY_min)
                        frog->y -= FROG_HEIGHT;
                    else
                    frog->y = validY_min;
                    frog->attached = 0;
                    frog->attached_crocodile_id = -1;
                    payload = *frog;
                    break;
                case KEY_DOWN:
                    if (frog->y + FROG_HEIGHT <= validY_max)
                        frog->y += FROG_HEIGHT;
                    else
                        frog->y = validY_max;
                    frog->attached = 0;
                    frog->attached_crocodile_id = -1;
                    payload = *frog;
                    break;
                case KEY_LEFT:
                    if (frog->x - FROG_WIDTH >= validX_min)
                        frog->x -= FROG_WIDTH;
                    else
                        frog->x = validX_min;
                    frog->attached = 0;
                    frog->attached_crocodile_id = -1;
                    payload = *frog;
                    break;
                case KEY_RIGHT:
                    if (frog->x + FROG_WIDTH <= validX_max)
                        frog->x += FROG_WIDTH;
                    else
                        frog->x = validX_max;
                    frog->attached = 0;
                    frog->attached_crocodile_id = -1;
                    payload = *frog;
                    break;
                case ' ':
                    payload.type = CREATE_GRENADE;
                    payload.x = frog->x;
                    payload.y = frog->y;

                    frog->attached = 0;
                    frog->attached_crocodile_id = -1;
                    
                    if (fork() == 0) {
                        Entity grenade_left;
                        createBullet(&grenade_left, payload.x - 1, payload.y, -1, 1);
                        grenade_left.pid = getpid();
                        bulletProcess(&grenade_left, fileds);
                        exit(EXIT_SUCCESS);
                    }
        
                    if (fork() == 0) {
                        Entity grenade_right;
                        createBullet(&grenade_right, payload.x + 1, payload.y, 1, 1);
                        grenade_right.pid = getpid();
                        bulletProcess(&grenade_right, fileds);
                        exit(EXIT_SUCCESS);
                    }
                    break;
            }
        }

        // Invia il payload SOLO se la rana NON è attaccata
        if (!frog->attached) {
            write(fileds[1], &payload, sizeof(Entity));
        }

        usleep(100000);
    }
}


