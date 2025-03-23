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
#include "collision.h"

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
    init_pair(4, COLOR_RED, COLOR_BLACK);
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

    int toFrog[2];
    if (pipe(toFrog) == -1) {
        perror("Errore creazione pipe");
        endwin();
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(fileds[0], F_GETFL, 0);
    fcntl(fileds[0], F_SETFL, flags | O_NONBLOCK);
    fcntl(toFrog[0], F_SETFL, flags | O_NONBLOCK);
    
    // Inizializza la rana (Entity per la rana)
    int taneOccupate[NUM_TANE] = {0};  // 0 = libera, 1 = occupata
    Entity frog;
    frog.type = OBJECT_FROG;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;
    
    // Calcola i limiti e inizializza l'area del fiume
int totalCrocs = LANES * CROCS_PER_LANE; // 16 Coccodrilli totali
Entity crocs[totalCrocs];
int index = 0;
int startCol = (COLS - PAVEMENT_WIDTH) / 2;
int endCol = startCol + PAVEMENT_WIDTH;
int riverStartRow = LINES - 27;  // Calcola il punto di partenza del fiume (come in drawRiver)

creaCrocodiles(crocs, startCol, endCol, riverStartRow);
    
    // Fork per la rana
    pid_t pid_frog = fork();
    if (pid_frog == -1) {
        perror("Errore fork rana");
        endwin();
        exit(EXIT_FAILURE);
    } else if (pid_frog == 0) {
        close(fileds[0]);
        frogProcess(&frog, fileds, toFrog);
        exit(EXIT_SUCCESS);
    }
    
// Fork per ogni CROCodrillo (ogni processo figlio gestisce un singolo CROCodrillo)
pid_t pid_crocs[totalCrocs];
for (int i = 0; i < totalCrocs; i++) {
    pid_crocs[i] = fork();
    if (pid_crocs[i] == -1) {
        perror("Errore fork CROCodrillo");
        endwin();
        exit(EXIT_FAILURE);
    } else if (pid_crocs[i] == 0) {
        close(fileds[0]);
        crocProcess(&crocs[i], fileds[1]);
        exit(EXIT_SUCCESS);
    }
}

   
    // Padre: chiude il lato di scrittura della pipe
    close(toFrog[0]);
    close(fileds[1]);
    
    // Ciclo principale: legge dalla pipe e ridisegna la scena senza usare clear()
    Entity temp;
    while (1) {
        if (read(fileds[0], &temp, sizeof(Entity)) > 0) {
            if (temp.type == OBJECT_FROG) { // Aggiorna la posizione della rana
        
                frog.y = temp.y;
                frog.x = temp.x;
    
            } else if (temp.type == OBJECT_CROCODILE) { // Aggiorna la posizione dei Coccodrilli
                if (!temp.inGioco) {
                    kill(pid_crocs[temp.id], SIGKILL);

                    pid_crocs[temp.id] = fork();
                    if (pid_crocs[temp.id] == -1) {
                        perror("Errore fork Coccodrillo");
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

        // verifica collisioni
        int tanaIndex = isFrogInTana(&frog);
        if (tanaIndex != -1) {
            taneOccupate[tanaIndex] = 1;  // Segna la tana come occupata
            frog.x = (COLS - FROG_WIDTH) / 2; // Riporta la rana al centro in basso
            frog.y = LINES - FROG_HEIGHT;
            write(toFrog[1], &frog, sizeof(Entity)); // Invia la nuova posizione
        }

        
 
        // Invece di chiamare clear(), ridisegniamo le aree statiche che "cancellano" le vecchie scritture: 
        // Ridisegna il fiume e il marciapiede: questi sovrascrivono l'area 
        drawRiver(); 
        drawPavement(); 
        drawMiddlePavement(); // marciapiede intermedio (uguale a quello basso)
        
        // La rana è già gestita tramite clearFrog() nel processo figlio, 
        // ma qui la ridisegniamo usando drawFrog() 
                // le 5 tane sopra il marciapiede intermedio


        // Disegna i Coccodrilli
         attron(COLOR_PAIR(4));
        for (int i = 0; i < totalCrocs; i++){
            if (!crocs[i].inGioco) continue;
            for (int row = 0; row < CROC_HEIGHT; row++) {
                for (int col = 0; col < CROC_WIDTH; col++) {
                    if (crocs[i].x + col >= ((COLS - PAVEMENT_WIDTH) / 2) && crocs[i].x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
                        mvprintw(crocs[i].y + row, crocs[i].x + col, "%c", SPRITE_CROC);
                }
            }
        }
         attroff(COLOR_PAIR(4));
        
        drawFrog(&frog);   
        drawTane(taneOccupate); 
         
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