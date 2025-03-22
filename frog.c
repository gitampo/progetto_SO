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
 
void frogProcess(Entity *frog, int pipeFD[2]) {
    frog->type = OBJECT_FROG;
    // Calcola i limiti validi per la rana basati sull'area del fiume/marciapiede
    int validX_min = (COLS - PAVEMENT_WIDTH) / 2;
    int validX_max = validX_min + PAVEMENT_WIDTH - FROG_WIDTH;
    int validY_min = LINES - 30.5;              // dove inizia il fiume
    int validY_max = LINES - FROG_HEIGHT;       // dove la rana finisce (per rimanere visibile)
    
    int isFrogInTana(const Entity *frog) {
        int taneRow = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;
        int startCol = (COLS - PAVEMENT_WIDTH) / 2;
        double slotWidth = (double)PAVEMENT_WIDTH / NUM_TANE + 0.5;
    
        if (frog->y == taneRow) { // Controlla se la rana è sulla riga delle tane
            for (int i = 0; i < NUM_TANE; i++) {
                int tanaStartCol = startCol + i * (int)slotWidth + (((int)slotWidth - LARGHEZZA_TANA) / 2);
                if (frog->x >= tanaStartCol && frog->x < tanaStartCol + LARGHEZZA_TANA) {
                    return i; // Restituisce l'indice della tana
                }
            }
        }
        return -1;  // Nessuna tana trovata
    }

    while (1) {
        int ch = getch();
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

        int tanaIndex = isFrogInTana(frog);
        if (tanaIndex != -1) {
            taneOccupate[tanaIndex] = 1;  // Segna la tana come occupata
            frog->x = (COLS - FROG_WIDTH) / 2; // Riporta la rana al centro in basso
            frog->y = LINES - FROG_HEIGHT;
        }
            
            write(pipeFD[1], frog, sizeof(Entity));
        }
        usleep(100000);
    }
}


