#include "graphics.h"
#include "frog.h"
#include <ncurses.h>
#include <unistd.h>
#include "bullet.h"
#include <stdlib.h>  // Per dichiarare exit() e EXIT_SUCCESS


extern int taneOccupate[NUM_TANE];  // Permette a frog.c di usare la variabile
 
void drawFrog(const Entity *frog) {

    attron(COLOR_PAIR(1));
    /*
    mvprintw(frog->y,     frog->x, SYMBOL_FROG_1); 
    mvprintw(frog->y + 1, frog->x, SYMBOL_FROG_2); 
    mvprintw(frog->y + 2, frog->x, SYMBOL_FROG_3); 
    */

    for (int col = 0; col < FROG_WIDTH; col++) {
        if (frog->x + col >= ((COLS - PAVEMENT_WIDTH) / 2) &&
            frog->x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
            mvprintw(frog->y, frog->x + col, "%c", SYMBOL_FROG_1[col]);
    }
    for (int col = 0; col < FROG_WIDTH; col++) {
        if (frog->x + col >= ((COLS - PAVEMENT_WIDTH) / 2) &&
            frog->x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
            mvprintw(frog->y + 1, frog->x + col, "%c", SYMBOL_FROG_2[col]);
    }
    for (int col = 0; col < FROG_WIDTH; col++) {
        if (frog->x + col >= ((COLS - PAVEMENT_WIDTH) / 2) &&
            frog->x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
            mvprintw(frog->y + 2, frog->x + col, "%c", SYMBOL_FROG_3[col]);
    }
    attroff(COLOR_PAIR(1));
} 
 
void clearFrog(const Entity *frog) { 
    for (int i = 0; i < FROG_HEIGHT; i++) { 
        mvprintw(frog->y + i, frog->x + i, "        "); // 8 spazi 
    } 
} 
 
void* frogThread(void *arg) {
    WINDOW *inputwin = newwin(1, 1, 0, 0);
    noecho();
    nodelay(inputwin, TRUE);
    keypad(inputwin, TRUE);

    frog_args *args = (frog_args *)arg;
    Entity *frog = &args->frog;
    entity_buffer *fileds = args->fileds;
    entity_buffer *toFrog = args->toFrog;

    frog->type = OBJECT_FROG;
    int validX_min = (COLS - PAVEMENT_WIDTH) / 2;
    int validX_max = validX_min + PAVEMENT_WIDTH - FROG_WIDTH;
    int validY_min = LINES - 33;              
    int validY_max = LINES - FROG_HEIGHT;       
    Entity temp;
    Entity payload = *frog; // Inizializza il payload con i dati della rana
    int onCroc = 0;
    int death = 0; // Flag per la morte della rana

    while (1) {
        int update = 1;
        
        if (read_from_buffer(toFrog, &temp)) {
            if (temp.type == FROG_ON_CROCODILE) {
                frog->x = temp.x; // La rana si posiziona sopra il coccodrillo
                onCroc = 1; // La rana è attaccata a un coccodrillo
                payload = *frog; // Invia la rana aggiornata al processo padre
            } else {
                frog->y = temp.y; // La rana si posiziona sopra il coccodrillo
                frog->x = temp.x; // La rana si posiziona sopra il coccodrillo
                onCroc = 0; // La rana non è attaccata a un coccodrillo
                death = 1; // La rana è morta
                payload = *frog; // Invia la rana aggiornata al processo padre
            }
        } 
        
        // Assicurati che il tipo sia sempre OBJECT_FROG
       
        //if (ch != ERR) {
        //pthread_mutex_lock(&mutex);
        //timeout(20);
            int ch = wgetch(inputwin); //getch(); // Leggi l'input dell'utente
        //pthread_mutex_unlock(&mutex);
            switch(ch) {
                case KEY_UP:
                    if (frog->y - FROG_HEIGHT >= validY_min)
                        frog->y -= FROG_HEIGHT;

                    else
                    frog->y = validY_min;
                    payload = *frog;
                    break;
                case KEY_DOWN:
                    if (frog->y + FROG_HEIGHT <= validY_max)
                        frog->y += FROG_HEIGHT;
                    else
                    frog->y = validY_max;
                    payload = *frog;
                    break;
                case KEY_LEFT:
                    if (frog->x - FROG_WIDTH >= validX_min)
                        frog->x -= FROG_WIDTH;
                    else
                    frog->x = validX_min;
                    payload = *frog;
                    break;
                case KEY_RIGHT:
                    if (frog->x + FROG_WIDTH <= validX_max)
                        frog->x += FROG_WIDTH;
                    else
                    frog->x = validX_max;
                    payload = *frog;
                    break;
                case ' ':
                    // Granata a sinistra
                    payload.type = CREATE_GRENADE;
                    payload.x = frog->x;  // Posizione centrale della rana
                    payload.y = frog->y + FROG_HEIGHT / 2;  // Posizione sopra la rana
                    break;

                case 'p':
                    payload.type = PAUSE_GAME;
                    break;

                default:
                    update = 0; // Mantieni la posizione corrente della rana
                    break;
            }
       
        if (update || onCroc || death) {
            write_from_buffer(fileds, &payload);
            death = 0; // Reset dello stato di morte
            onCroc = 0; // Reset dello stato di attacco
        }
   
        if (frog ->y == validY_min) {
            // La rana ha raggiunto la tana
            
            frog->x = (COLS - FROG_WIDTH) / 2;
            frog->y = LINES - FROG_HEIGHT;
            payload = *frog; // Invia la rana aggiornata al processo padre
            write_from_buffer(fileds, &payload);
        }

       
    }

     
}
