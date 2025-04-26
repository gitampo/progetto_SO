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
#include "bullet.h"

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
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    nodelay(stdscr, TRUE);

    // Creazione delle pipe
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

    // Inizializzazione della rana
    int taneOccupate[NUM_TANE] = {0};  // 0 = libera, 1 = occupata
    Entity frog;
    frog.type = OBJECT_FROG;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;
<<<<<<< HEAD
    int frogOnCrocodile = 0;
    int crocodileSpeed = 0;

    
    // Calcola i limiti e inizializza l'area del fiume
    int totalCrocs = LANES * CROCS_PER_LANE; // 16 Coccodrilli totali
=======

    // Inizializzazione dei coccodrilli
    int totalCrocs = LANES * CROCS_PER_LANE;
>>>>>>> 7dd54a68c627de98bd24141408241068ca4d13df
    Entity crocs[totalCrocs];
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    int riverStartRow = LINES - 27;
    int totalGrenades = 20;

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
<<<<<<< HEAD
    
    // Fork per ogni coccodrillo (ogni processo figlio gestisce un singolo CROCodrillo)
    creaCrocodiles(crocs, startCol, endCol, riverStartRow);
=======
>>>>>>> 7dd54a68c627de98bd24141408241068ca4d13df

    creaCrocodiles(crocs, startCol, endCol, riverStartRow);
    pid_t pid_crocs[totalCrocs];  // Array per memorizzare i pid dei coccodrilli
    for (int i = 0; i < totalCrocs; i++) {
        pid_crocs[i] = fork();
        if (pid_crocs[i] == -1) {
            perror("Errore fork Coccodrillo");
            endwin();
            exit(EXIT_FAILURE);
        } else if (pid_crocs[i] == 0) {
            close(fileds[0]);  // Ogni processo figlio deve chiudere il lato di lettura
            crocProcess(&crocs[i], fileds[1]);  // Ogni figlio gestisce un coccodrillo
            exit(EXIT_SUCCESS);  // Ogni processo figlio termina dopo aver eseguito il proprio lavoro
        }
    }
    
  
    // Inizializzazione dell'array per i bullet
    Entity bullets[MAX_BULLETS];  // Usa MAX_BULLETS invece di MAX_PROJECTILES
    for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
        bullets[i].inGioco = 0;
    }

    // Chiusura degli estremi non utilizzati nelle pipe
    close(toFrog[0]);
    close(fileds[1]);

    Entity temp;
    while (1) {
        frogOnCrocodile = 0;
        if (read(fileds[0], &temp, sizeof(Entity)) > 0) {
            if (temp.type == OBJECT_FROG) {
                frog.y = temp.y;
                frog.x = temp.x;
            } else if (temp.type == OBJECT_CROCODILE) {
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
                        crocs[temp.id].x = (temp.direction == 1) ? crocs[temp.id].initX : crocs[temp.id].initX;
                        crocProcess(&crocs[temp.id], fileds[1]);
                        exit(EXIT_SUCCESS);
                    }
                } else {
                    crocs[temp.id].y = temp.y;
                    crocs[temp.id].x = temp.x;
                }
            
            } else if (temp.type == OBJECT_BULLET) {  // Usa OBJECT_BULLET invece di OBJECT_PROJECTILE
                // Gestione dei bullet: aggiorna o inserisce nell'array bullets
                int found = -1;
                for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
                    if (bullets[i].id == temp.id) {
                        found = i;
                        break;
                    }
                }
                if (found != -1) {
                    bullets[found] = temp;
                } else {
                    for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
                        if (bullets[i].inGioco == 0) {
                            bullets[i] = temp;
                            break;
                        }
                    }
                }
            }
        }

        // Verifica se la rana è entrata in una tana
        int tanaIndex = isFrogInTana(&frog);
        if (tanaIndex != -1) {
            taneOccupate[tanaIndex] = 1;
            frog.x = (COLS - FROG_WIDTH) / 2;
            frog.y = LINES - FROG_HEIGHT;
            write(toFrog[1], &frog, sizeof(Entity));
        }

<<<<<<< HEAD
        // Invece di chiamare clear(), ridisegniamo le aree statiche che "cancellano" le vecchie scritture: 
        // Ridisegna il fiume e il marciapiede: questi sovrascrivono l'area 
         // Cancella la rana dalla sua posizione precedente
        drawRiver(); 
        drawPavement(); 
        drawMiddlePavement(); // marciapiede intermedio (uguale a quello basso)
        
        // La rana è già gestita tramite clearFrog() nel processo figlio, 
        // ma qui la ridisegniamo usando drawFrog() 
        // le 5 tane sopra il marciapiede intermedio

        // Disegna i Coccodrilli
=======
        // Ridisegno delle aree statiche
        drawRiver();
        drawPavement();
        drawMiddlePavement();

        // Disegno dei coccodrilli
>>>>>>> 7dd54a68c627de98bd24141408241068ca4d13df
        attron(COLOR_PAIR(4));
        for (int i = 0; i < totalCrocs; i++) {
            if (!crocs[i].inGioco) continue;
            for (int row = 0; row < CROC_HEIGHT; row++) {
                for (int col = 0; col < CROC_WIDTH; col++) {
                    if (crocs[i].x + col >= ((COLS - PAVEMENT_WIDTH) / 2) &&
                        crocs[i].x + col < ((COLS - PAVEMENT_WIDTH) / 2) + PAVEMENT_WIDTH)
                        mvprintw(crocs[i].y + row, crocs[i].x + col, "%c", SPRITE_CROC);
                }
            }
        }
        attroff(COLOR_PAIR(4));

<<<<<<< HEAD
        for (int i = 0; i < totalCrocs; i++) {
            if (!crocs[i].inGioco) continue;
            if (crocs[i].y == frog.y && crocs[i].x == frog.x) {
                frogOnCrocodile = 1;
                crocodileSpeed = crocs[i].speed;
            } else if (crocs[i].y == frog.y && crocs[i].x + FROG_WIDTH >= frog.x && crocs[i].x <= frog.x + FROG_WIDTH) {
                frogOnCrocodile = 1;
                crocodileSpeed = crocs[i].speed;
            }
            
        }

        
        
=======
>>>>>>> 7dd54a68c627de98bd24141408241068ca4d13df
        drawVoid();
        drawFrog(&frog);
        drawTane(taneOccupate);

       

        // Disegno dei bullet
        for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
            if (bullets[i].inGioco) {
                drawBullet(&bullets[i]);  // Usa drawBullet
            }
        }
        refresh();
    }

    endwin();
    kill(pid_frog, SIGKILL);
    for (int i = 0; i < totalCrocs; i++){
        kill(pid_crocs[i], SIGKILL);
    }
    wait(NULL);
    wait(NULL);
    return 0;
}
