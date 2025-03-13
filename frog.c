#include "frog.h" 
#include <ncurses.h> 
#include <unistd.h> 
 
void drawFrog(const Entity *frog) { 
    mvprintw(frog->y,     frog->x, SYMBOL_FROG_1); 
    mvprintw(frog->y + 1, frog->x, SYMBOL_FROG_2); 
    mvprintw(frog->y + 2, frog->x, SYMBOL_FROG_3); 
} 
 
void clearFrog(const Entity *frog) { 
    for (int i = 0; i < FROG_HEIGHT; i++) { 
        mvprintw(frog->y + i, frog->x, "        "); // 8 spazi 
    } 
} 
 
void frogProcess(Entity *frog, int fileds[2]) { 
    // Imposta il tipo e l'ID per la rana 
    frog->type = OBJECT_FROG; 
    // La rana manterrà il salto pari alle sue dimensioni: 
    while (1) { 
        int ch = getch(); 
        if (ch != ERR) { 
            switch(ch) { 
                case KEY_UP: 
                    if (frog->y >= FROG_HEIGHT) frog->y -= FROG_HEIGHT; 
                    break; 
                case KEY_DOWN: 
                    if (frog->y <= LINES - FROG_HEIGHT - 1) frog->y += FROG_HEIGHT; 
                    break; 
                case KEY_LEFT: 
                    if (frog->x >= FROG_WIDTH) frog->x -= FROG_WIDTH; 
                    break; 
                case KEY_RIGHT: 
                    if (frog->x <= COLS - FROG_WIDTH - 1) frog->x += FROG_WIDTH; 
                    break; 
            } 
            write(fileds[1], frog, sizeof(Entity)); 
        } 
        usleep(10000); 
    } 
}