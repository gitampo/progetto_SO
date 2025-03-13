#include "graphics.h" 
#include <ncurses.h> 
 
// Disegna il marciapiede centrato in basso con colore pair 3 
void drawPavement() { 
    attron(COLOR_PAIR(3)); 
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
 
// Disegna il fiume con altezza RIVER_HEIGHT e colore pair 2. 
// Qui il fiume viene disegnato a partire da LINES-27 (ad es. se LINES=30, allora da riga 3 a 26). 
void drawRiver() { 
    attron(COLOR_PAIR(2)); 
    int startRow = LINES - 27; 
    int endRow   = startRow + RIVER_HEIGHT; 
    int startCol = (COLS - PAVEMENT_WIDTH) / 2; 
    int endCol   = startCol + PAVEMENT_WIDTH; 
    if (endRow > LINES) { 
        endRow = LINES; 
    } 
    for (int r = startRow; r < endRow; r++) { 
        for (int c = startCol; c < endCol; c++) { 
            mvaddch(r, c, '~'); 
        } 
    } 
    attroff(COLOR_PAIR(2)); 
}