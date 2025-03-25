#include "graphics.h"
#include "frog.h"
#include <ncurses.h>
#include <unistd.h>

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
        mvprintw(frog->y + i, frog->x, "        "); // 8 spazi 
    } 
} 
 
void frogProcess(Entity *frog, int pipeFD[2], int toFrog[2]) {
    frog->type = OBJECT_FROG;
    // Calcola i limiti validi per la rana basati sull'area del fiume/marciapiede
    int validX_min = (COLS - PAVEMENT_WIDTH) / 2;
    int validX_max = validX_min + PAVEMENT_WIDTH - FROG_WIDTH;
    int validY_min = LINES - 33;              // dove inizia il fiume
    int validY_max = LINES - FROG_HEIGHT;       // dove la rana finisce (per rimanere visibile)
    Entity temp;
    

    while (1) {
        int ch = getch();

        if (read(toFrog[0], &temp, sizeof(Entity)) > 0) {
            frog->y = temp.y;
            frog->x = temp.x;
        }
        
        if (ch != ERR) {
            switch(ch) {
                case KEY_UP:
                    if (frog->y - FROG_HEIGHT >= validY_min)
                        frog->y -= FROG_HEIGHT;
                    else
                        frog->y = validY_min;
                    break;
                case KEY_DOWN:
                    if (frog->y + FROG_HEIGHT <= validY_max)
                        frog->y += FROG_HEIGHT;
                    else
                        frog->y = validY_max;
                    break;
                case KEY_LEFT:
                    if (frog->x - FROG_WIDTH >= validX_min)
                        frog->x -= FROG_WIDTH;
                    else
                        frog->x = validX_min;
                    break;
                case KEY_RIGHT:
                    if (frog->x + FROG_WIDTH <= validX_max)
                        frog->x += FROG_WIDTH;
                    else
                        frog->x = validX_max;
                    break;
            }
            
        // Dentro il loop della rana
        
        write(pipeFD[1], frog, sizeof(Entity));
        
        }
        usleep(100000);
    }
}


