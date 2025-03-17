#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <signal.h> 
#include <ncurses.h> 
#include <fcntl.h> // Per O_NONBLOCK 
#include "graphics.h" 
#include "frog.h" 
#include "croc.h" 
#include "entity.h"   
 
int main() { 
    initscr(); 
    cbreak(); 
    noecho(); 
    keypad(stdscr, TRUE); 
    curs_set(0); 
     
    start_color(); 
    init_pair(1, COLOR_GREEN, COLOR_BLACK); 
    init_pair(2, COLOR_BLUE, COLOR_BLUE); 
    init_pair(3, COLOR_RED, COLOR_YELLOW); 
    init_pair(4, COLOR_WHITE, COLOR_BLACK); 
    if (can_change_color()) { 
        init_color(COLOR_YELLOW, 500, 250, 0); 
    } 
     
    nodelay(stdscr, TRUE); 
     
    
    // Creazione della pipe: lato 0 per leggere, lato 1 per scrivere 
    int fileds[2]; 
    if (pipe(fileds) == -1) { 
        perror("Errore creazione pipe"); 
        endwin(); 
        exit(EXIT_FAILURE); 
    } 
    int flags = fcntl(fileds[0], F_GETFL, 0); 
    fcntl(fileds[0], F_SETFL, flags | O_NONBLOCK); 
     
    // Inizializza la rana (Entity per la rana) 
    Entity frog; 
    frog.id = 1; 
    frog.type = OBJECT_FROG; 
    frog.y = LINES - FROG_HEIGHT;  
    frog.x = (COLS - FROG_WIDTH) / 2; 
     
    // Calcola il punto di partenza del fiume (come in drawRiver) 
    int riverStartRow = LINES - 27; 
     
    // Inizializza i coccodrilli all'interno del fiume: 
    // Il fiume è alto RIVER_HEIGHT (24), ogni coccodrillo è alto COCC_HEIGHT (3) 
    // ⇒ Numero di corsie = 24/3 = 8, con 2 coccodrilli per corsia (totale 16) 
    int lanes = RIVER_HEIGHT / COCC_HEIGHT;  // 8 corsie 
    int crocsPerLane = 2; 
    int totalCrocs = lanes * crocsPerLane; // 16 
    Entity crocs[totalCrocs]; 
    int index = 0; 
    int startCol = (COLS - PAVEMENT_WIDTH) / 2; 
    int endCol   = startCol + PAVEMENT_WIDTH; 
    for (int i = 0; i < lanes; i++) { 
        for (int j = 0; j < crocsPerLane; j++) { 
            crocs[index].id = index + 2; // Rana è 1, crocs iniziano da 2 
            crocs[index].type = OBJECT_CROCODILE; 
            crocs[index].y = riverStartRow + i * COCC_HEIGHT; 
            // Direzione alternata: corsia pari -> muovi a destra; dispari -> a sinistra 
            int dir = (i % 2 == 0) ? 1 : -1; 
            if (dir == 1) 
                crocs[index].x = startCol + j * 10; 
            else 
                crocs[index].x = endCol - COCC_WIDTH - j * 10; 
            // Per il respawn potresti voler salvare la posizione iniziale in un campo specifico; 
            // in questa versione, lasciamo la logica del respawn in runCrocs() come da implementazione. 
            index++; 
        } 
    } 
     
    // Fork per la rana 
    pid_t pid_frog = fork(); 
    if (pid_frog == -1) { 
        perror("Errore fork rana"); 
        endwin(); 
        exit(EXIT_FAILURE); 
    } else if (pid_frog == 0) { 
        close(fileds[0]); 
        frogProcess(&frog, fileds[1]); 
        exit(EXIT_SUCCESS); 
    } 
     
    // Fork per i coccodrilli 
    pid_t pid_crocs = fork(); 
    if (pid_crocs == -1) { 
        perror("Errore fork coccodrilli"); 
        endwin(); 
        exit(EXIT_FAILURE); 
    } else if (pid_crocs == 0) { 
        close(fileds[0]); 
        runCrocs(crocs, totalCrocs, fileds[1]); 
        exit(EXIT_SUCCESS); 
    } 
     
    // Padre: chiude il lato di scrittura della pipe 
    close(fileds[1]); 
     
    // Ciclo principale: legge dalla pipe e ridisegna la scena senza usare clear() 
    Entity temp; 
    while (1) { 
        if (read(fileds[0], &temp, sizeof(Entity)) > 0) {
            if (temp.id == 1) { // Aggiorna la posizione della guardia
                mvaddch(frog.y, frog.x, ' ');
                frog.y = temp.y;
                frog.x = temp.x;
                mvaddch(frog.y, frog.x, SYMBOL_FROG_1);
                mvaddch(frog.y, frog.x, SYMBOL_FROG_2);
                mvaddch(frog.y, frog.x, SYMBOL_FROG_3);
            } else if (temp.id == 2) { // Aggiorna la posizione del ladro
                for(int i = 0; i < )
                mvaddch(croc.y, croc.x, ' ');
                croc.y = temp.y;
                croc.x = temp.x;
                mvaddch(croc.y, croc.x, SPRITE_CROC);
            }
        }

 
        // Invece di chiamare clear(), ridisegniamo le aree statiche che "cancellano" le vecchie scritture: 
        // Ridisegna il fiume e il marciapiede: questi sovrascrivono l'area 
        drawRiver(); 
        drawPavement(); 
         
        // La rana è già gestita tramite clearFrog() nel processo figlio, 
        // ma qui la ridisegniamo usando drawFrog() 
        drawFrog(&frog); 
         
        // Disegna i coccodrilli 
        attron(COLOR_PAIR(4)); 
        for (int i = 0; i < totalCrocs; i++) { 
            for (int row = 0; row < COCC_HEIGHT; row++) { 
                for (int col = 0; col < COCC_WIDTH; col++) { 
                    mvaddch(crocs[i].y + row, crocs[i].x + col, SPRITE_CROC); 
                } 
            } 
        } 
        attroff(COLOR_PAIR(4)); 
         
        refresh(); 
        usleep(70000); // circa 14 FPS 
    } 
     
    endwin(); 
    kill(pid_frog, SIGKILL); 
    kill(pid_crocs, SIGKILL); 
    wait(NULL); 
    wait(NULL); 
    return 0; 
}