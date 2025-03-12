#include "graphics.h"
#include <ncurses.h>

// Disegna il marciapiede centrato in basso (ESISTENTE, se già definito)
void drawPavement() {
    attron(COLOR_PAIR(3)); // colore diverso per il marciapiede
    int startRow = LINES - PAVEMENT_HEIGHT;
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol   = startCol + PAVEMENT_WIDTH;

    for (int r = startRow; r < LINES; r++) {
        for (int c = startCol; c < endCol; c++) {
            mvaddch(r, c, '='); 
        }
    }

    attroff(COLOR_PAIR(3));
}


// Disegna il fiume alto 24 righe, largo quanto il marciapiede, centrato orizzontalmente
void drawRiver() {
    attron(COLOR_PAIR(2)); // colore diverso per il fiume
    int startRow = LINES - 27;                       // disegna il fiume a partire dalla riga 0 (top dello schermo)
    int endRow   = startRow + RIVER_HEIGHT; // startRow + 24
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol   = startCol + PAVEMENT_WIDTH;

    // Se RIVER_HEIGHT supera lo schermo, potresti mettere un check:
    if (endRow > LINES) {
        endRow = LINES; // evita di sforare
    }

    // Riempie l'area con il carattere '~' (simbolo di acqua)
    for (int r = startRow; r < endRow; r++) {
        for (int c = startCol; c < endCol; c++) {
            mvaddch(r, c, '~');
        }
    }
    attroff(COLOR_PAIR(2));
}
