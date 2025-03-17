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
    frog.type = OBJECT_FROG;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;
    
    // Calcola i limiti e inizializza l'area del fiume
int lanes = RIVER_HEIGHT / COCC_HEIGHT;  // 8 corsie
int crocsPerLane = 2;
int totalCrocs = lanes * crocsPerLane; // 16 coccodrilli totali
Entity crocs[totalCrocs];
int index = 0;
int startCol = (COLS - PAVEMENT_WIDTH) / 2;
int endCol = startCol + PAVEMENT_WIDTH;
int riverStartRow = LINES - 27;  // Calcola il punto di partenza del fiume (come in drawRiver)

for (int i = 0; i < lanes; i++) {
    int dir = (i % 2 == 0) ? 1 : -1;  // corsia pari: da sinistra a destra; dispari: da destra a sinistra
    for (int j = 0; j < crocsPerLane; j++) {
        crocs[index].id = index; // Rana ha id 1, coccodrilli da 2
        crocs[index].type = OBJECT_CROCODILE;
        crocs[index].y = riverStartRow + i * COCC_HEIGHT;
        crocs[index].direction = dir;
        if (dir == 1)
            crocs[index].x = startCol + j * 10;
        else
            crocs[index].x = endCol - COCC_WIDTH - j * 10;
        crocs[index].initX = crocs[index].x;
        crocs[index].inGioco = 1; // Imposta inGioco a 1
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
        frogProcess(&frog, fileds);
        exit(EXIT_SUCCESS);
    }
    
// Fork per ogni coccodrillo (ogni processo figlio gestisce un singolo coccodrillo)
pid_t pid_crocs[totalCrocs];
for (int i = 0; i < totalCrocs; i++) {
    pid_crocs[i] = fork();
    if (pid_crocs[i] == -1) {
        perror("Errore fork coccodrillo");
        endwin();
        exit(EXIT_FAILURE);
    } else if (pid_crocs[i] == 0) {
        close(fileds[0]);
        crocProcess(&crocs[i], fileds[1]);
        exit(EXIT_SUCCESS);
    }
}


   
    // Padre: chiude il lato di scrittura della pipe
    close(fileds[1]);
    
    // Ciclo principale: legge dalla pipe e ridisegna la scena senza usare clear()
    Entity temp;
    while (1) {
        if (read(fileds[0], &temp, sizeof(Entity)) > 0) {
            if (temp.type == OBJECT_FROG) { // Aggiorna la posizione della rana
        
                frog.y = temp.y;
                frog.x = temp.x;
    
            } else if (temp.type == OBJECT_CROCODILE) { // Aggiorna la posizione dei coccodrilli
                if (!temp.inGioco) {
                    kill(pid_crocs[temp.id], SIGKILL);

                    pid_crocs[temp.id] = fork();
                    if (pid_crocs[temp.id] == -1) {
                        perror("Errore fork coccodrillo");
                        endwin();
                        exit(EXIT_FAILURE);
                    } else if (pid_crocs[temp.id] == 0) {

                        close(fileds[0]);
                        crocs[temp.id].inGioco = 1;
                        crocs[temp.id].x = 
                        temp.direction == 1 ?
                            crocs[temp.id].initX
                            : crocs[temp.id].initX;
                        crocProcess(&crocs[temp.id], fileds[1]);
                        exit(EXIT_SUCCESS);
                    }

                } else {
                    crocs[temp.id].y = temp.y;
                    crocs[temp.id].x = temp.x; 
                } 
            } 
        } 
 
        // Invece di chiamare clear(), ridisegniamo le aree statiche che "cancellano" le vecchie scritture: 
        // Ridisegna il fiume e il marciapiede: questi sovrascrivono l'area 
        drawRiver(); 
        drawPavement(); 
         
        // La rana è già gestita tramite clearFrog() nel processo figlio, 
        // ma qui la ridisegniamo usando drawFrog() 
        drawFrog(&frog); 

        

         
         attron(COLOR_PAIR(4));
        for (int i = 0; i < totalCrocs; i++){
            if (!crocs[i].inGioco) continue;
            for (int row = 0; row < COCC_HEIGHT; row++) {
                for (int col = 0; col < COCC_WIDTH; col++) {
                    if (crocs[i].x + col >= ((COLS - PAVEMENT_WIDTH) / 2) && crocs[i].x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
                        mvprintw(crocs[i].y + row, crocs[i].x + col, "%c", SPRITE_CROC);
                }
            }
        }
 
 attroff(COLOR_PAIR(4));
        
         
        refresh(); 
        
    } 
     
    endwin(); 
    kill(pid_frog, SIGKILL); 
    for (int i = 0; i<totalCrocs; i++){
        kill(pid_crocs[i], SIGKILL);
    }
    wait(NULL); 
    wait(NULL); 
    return 0; 
}