#include "graphics.h"
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "frog.h"



int taneOccupate[NUM_TANE] = {0};  // 0 = libera, 1 = occupata




// Disegna il marciapiede centrato in basso (già esistente)
void drawPavement() {
    attron(COLOR_PAIR(3));
    int startRow = LINES - PAVEMENT_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    for (int r = startRow; r < LINES; r++) {
        for (int c = startCol; c < endCol; c++) {
            mvprintw(r, c, "=");
        }
    }
    attroff(COLOR_PAIR(3));
}

// Disegna il fiume con altezza RIVER_HEIGHT (da LINES-27 a LINES-27+RIVER_HEIGHT)
void drawRiver() {
    attron(COLOR_PAIR(2));
    int startRow = LINES - 27;
    int endRow = startRow + RIVER_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    if (endRow > LINES) {
        endRow = LINES;
    }
    for (int r = startRow; r < endRow; r++) {
        for (int c = startCol; c < endCol; c++) {
            mvprintw(r, c, " ");
        }
    }
    attroff(COLOR_PAIR(2));
}

// Disegna il marciapiede intermedio, subito sopra il fiume
void drawMiddlePavement() {
    attron(COLOR_PAIR(3));
    int startRow = (LINES - 27) - MIDDLE_SIDEWALK_HEIGHT;
    int endRow = (LINES - 27);
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    for (int r = startRow; r < endRow; r++) {
        for (int c = startCol; c < endCol; c++) {
            mvprintw(r, c, "=");
        }
    }
    attroff(COLOR_PAIR(3));
}

void drawTane(int statusTane[NUM_TANE]) {
    
    int taneRow = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2; // Inizia un po' più a sinistra per centrare
    double slotWidth = (double)PAVEMENT_WIDTH / NUM_TANE; // Calcola la larghezza di ogni slot
    
    for (int i = 0; i < NUM_TANE; i++) {
        int tanaStartCol = startCol + i * (int)slotWidth + (((int)slotWidth - LARGHEZZA_TANA) / 2);
            if (statusTane[i]) {
                attron(COLOR_PAIR(3));
                mvprintw(taneRow, tanaStartCol, "        ");
                mvprintw(taneRow + 1, tanaStartCol, "        ");
                mvprintw(taneRow + 2, tanaStartCol, "        ");
                attroff(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(5));
                // Stampa un singolo carattere per ogni riga della sprite della tana
                mvprintw(taneRow, tanaStartCol, "%s", SYMBOL_TANA_1);
                mvprintw(taneRow + 1, tanaStartCol, "%s", SYMBOL_TANA_2);
                mvprintw(taneRow + 2, tanaStartCol, "%s", SYMBOL_TANA_3);
                attroff(COLOR_PAIR(5));
            }
    }
}

void drawVoid() {
    attron(COLOR_PAIR(6));
    int startRow = LINES - 27 - PAVEMENT_HEIGHT - TANE_HEIGHT;
    int endRow = startRow + TANE_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    for (int r = startRow; r < endRow; r++) {
        for (int c = startCol; c < endCol; c++) {
            mvprintw(r, c, " ");
        }
    }
    attroff(COLOR_PAIR(6));
}


// Funzione per disegnare un bullet
void drawBullet(Entity *bullet) {
    if (bullet->inGioco) {
        if (bullet->x >= ((COLS - PAVEMENT_WIDTH) / 2) &&
           bullet->x < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH){
        attron(COLOR_PAIR(7));  // Usa un colore visibile
        mvaddch(bullet->y, bullet->x, '*');
        attroff(COLOR_PAIR(7));
     }
    }
}

void drawGrenade(Entity *grenade) {
    if (grenade->x >= ((COLS - PAVEMENT_WIDTH) / 2) &&
        grenade->x < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH){
    attron(COLOR_PAIR(7));
    mvprintw(grenade->y, grenade->x, grenade->direction ==-1 ? "<" : ">"); 
    attroff(COLOR_PAIR(7));
 }
}


void drawTimer(int timeLeft, int maxTime, int bar_row, int bar_colStart) {
    attron(COLOR_PAIR(5));
    
    for (int i = 0; i < timeLeft; i++) {
        mvprintw(bar_row, bar_colStart + i, "#"); // Cancella la barra precedente
    }
    attroff(COLOR_PAIR(5));
    for (int i = timeLeft; i < maxTime - timeLeft; i++) {
        mvprintw(bar_row, bar_colStart + i, " "); // Cancella la barra precedente
    }
    
}   