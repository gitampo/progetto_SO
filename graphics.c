#include "graphics.h"
#include <ncurses.h>

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
            mvprintw(r, c, "~");
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
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    double slotWidth = (double)PAVEMENT_WIDTH / NUM_TANE;
    
    for (int i = 0; i < NUM_TANE; i++) {
        int tanaStartCol = startCol + i * (int)slotWidth + (((int)slotWidth - LARGHEZZA_TANA) / 2);
        for (int j = 0; j < LARGHEZZA_TANA; j++) {
            if (statusTane[i]) {

                attron(COLOR_PAIR(3));
                mvaddch(taneRow, tanaStartCol + j, ' ');
                mvaddch(taneRow + 1, tanaStartCol + j, ' ');
                mvaddch(taneRow + 2, tanaStartCol + j, ' ');
                attroff(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(5));
                // Stampa un singolo carattere per ogni riga della sprite della tana
                mvaddch(taneRow, tanaStartCol + j, SYMBOL_TANA_1[j]);
                mvaddch(taneRow + 1, tanaStartCol + j, SYMBOL_TANA_2[j]);
                mvaddch(taneRow + 2, tanaStartCol + j, SYMBOL_TANA_3[j]);
                attroff(COLOR_PAIR(5));
            }
        }
    }
   
}



