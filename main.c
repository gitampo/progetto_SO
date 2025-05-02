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
    signal(SIGCHLD, SIG_IGN); // Ignora i segnali SIGCHLD per evitare zombie
    
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
    int frogflags = fcntl(toFrog[0], F_GETFL, 0);

    fcntl(fileds[0], F_SETFL, O_NONBLOCK);
    fcntl(toFrog[0], F_SETFL, O_NONBLOCK);

    // Inizializzazione della rana
    int taneOccupate[NUM_TANE] = {0};  // 0 = libera, 1 = occupata
    Entity frog;
    frog.attached = 0; // 0 se non attaccato, 1 se attaccato
    frog.attached_crocodile_id = -1; // ID del coccodrillo a cui è attaccato
    frog.type = OBJECT_FROG;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;
    int crocodileSpeed = 0;
    int validX_min = (COLS - PAVEMENT_WIDTH) / 2;
    int validX_max = validX_min + PAVEMENT_WIDTH - FROG_WIDTH;
    int validY_min = LINES - 33;
    int validY_max = LINES - FROG_HEIGHT;
    int frog_on_crocodile = -1;
    
    // Calcola i limiti e inizializza l'area del fiume
    int totalCrocs = LANES * CROCS_PER_LANE; // 16 Coccodrilli totali
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
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        timeout(0);
        close(fileds[0]);
        frogProcess(&frog, fileds, toFrog);
        exit(EXIT_SUCCESS);
    }

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
            crocProcess(&crocs[i], fileds[1], riverStartRow);  // Ogni figlio gestisce un coccodrillo
            exit(EXIT_SUCCESS);  // Ogni processo figlio termina dopo aver eseguito il proprio lavoro
        }
    }
    
  
    // Inizializzazione dell'array per i bullet
    Entity bullets[MAX_BULLETS];  // Usa MAX_BULLETS invece di MAX_PROJECTILES
    pid_t pid_bullets[MAX_BULLETS];  // Array per memorizzare i pid dei proiettili
    int i_last_bullet = 0; // Indice dell'ultimo proiettile creato
    for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
        bullets[i].inGioco = 0;
    }

    Entity grenades[MAX_GRENADES];  // Usa MAX_BULLETS invece di MAX_PROJECTILES
    pid_t pid_grenades[MAX_GRENADES];  // Array per memorizzare i pid dei proiettili
    int i_last_grenades = 0; // Indice dell'ultimo proiettile creato
    for (int i = 0; i < MAX_GRENADES; i++) {  // Usa MAX_BULLETS
        grenades[i].inGioco = 0;
    }

    Entity temp;
    while (1) {
        if (read(fileds[0], &temp, sizeof(Entity)) > 0) {
            if (temp.type == OBJECT_FROG) {
                frog.y = temp.y;
                frog.x = temp.x;
            } else if (temp.type == OBJECT_CROCODILE) {

                crocs[temp.id].y = temp.y;
                crocs[temp.id].x = temp.x;
                if (temp.id == frog_on_crocodile) {
                    Entity frog_temp = frog;
                    frog_temp.type = FROG_ON_CROCODILE;
                    frog_temp.x = crocs[temp.id].x + (CROC_WIDTH / 2) - (FROG_WIDTH / 2); // La rana si posiziona sopra il coccodrillo
                    write(toFrog[1], &frog_temp, sizeof(Entity));
                }
            
            } else if (temp.type == CREATE_BULLET) {
                // Creazione del proiettile
                temp.type = OBJECT_BULLET; // Imposta il tipo di proiettile
                temp.id = i_last_bullet; // Imposta l'ID del proiettile
                bullets[i_last_bullet] = temp; // Aggiungi il proiettile all'array
                
                pid_bullets[i_last_bullet] = fork();
                if (pid_bullets[i_last_bullet] == -1) {
                    perror("Errore fork Proiettile");
                    endwin();
                    exit(EXIT_FAILURE);
                } else if (pid_bullets[i_last_bullet] == 0) {
                    close(fileds[0]);  // Ogni processo figlio deve chiudere il lato di lettura
                    bulletProcess(&temp, fileds[1]);  // Ogni figlio gestisce un coccodrillo
                    exit(EXIT_SUCCESS);  // Ogni processo figlio termina dopo aver eseguito il proprio lavoro
                } 
                i_last_bullet = (i_last_bullet + 1) % MAX_BULLETS; // Incrementa l'indice dell'ultimo proiettile creato
            } else if (temp.type == OBJECT_BULLET) {
                // exit(EXIT_SUCCESS); // Termina il processo figlio del proiettile
                if (temp.inGioco) {
                    bullets[temp.id] = temp; // Aggiungi il proiettile all'array
                } else {
                    // Rimuovi il proiettile dall'array
                    bullets[temp.id].inGioco = 0;
                    kill(pid_bullets[temp.id], SIGTERM);
                }

              
            } else if (temp.type == CREATE_GRENADE) {
                int grenadesx[2] = {temp.x -1, temp.x + FROG_WIDTH};
                int grenade_direction[2] = { -1, 1}; // Direzione della granata (sinistra e destra)
                // Creazione della granata

                for (int i = 0; i < 2; i++)
                {
                    
                    temp.id = i_last_grenades; // Imposta l'ID del proiettile
                    temp.x = grenadesx[i]; // Imposta la posizione della granata
                    temp.type = OBJECT_GRENADE; // Imposta il tipo di proiettile
                    temp.inGioco = 1; // Imposta lo stato della granata a "in gioco"
                    temp.speed = 2; // Imposta la velocità della granata
                    temp.direction = grenade_direction[i]; // Imposta la direzione della granata
                    grenades[i_last_grenades] = temp; // Aggiungi il proiettile all'array
                    
    
                    
                    pid_grenades[i_last_grenades] = fork();
                    if (pid_grenades[i_last_grenades] == -1) {
                        perror("Errore fork Granata");
                        endwin();
                        exit(EXIT_FAILURE);
                    } else if (pid_grenades[i_last_grenades] == 0) {
                        close(fileds[0]);  // Ogni processo figlio deve chiudere il lato di lettura
                        grenadeProcess(&temp, fileds[1]);  // Ogni figlio gestisce un coccodrillo
                        exit(EXIT_SUCCESS);  // Ogni processo figlio termina dopo aver eseguito il proprio lavoro
                    } 
                    i_last_grenades = (i_last_grenades + 1) % MAX_GRENADES; // Incrementa l'indice dell'ultimo proiettile creato
                }
            }

            else if (temp.type == OBJECT_GRENADE) {
                // exit(EXIT_SUCCESS); // Termina il processo figlio del proiettile
                if (temp.inGioco) {
                    grenades[temp.id] = temp; // Aggiungi il proiettile all'array
                } else {
                    // Rimuovi il proiettile dall'array
                    grenades[temp.id].inGioco = 0;
                    kill(pid_grenades[temp.id], SIGTERM);
                }
            }
        } 
      
        for(int i = 0; i < totalCrocs; i++) {
         
            if(
                (frog.y == crocs[i].y) && 
                (
                    (inBetween(frog.x, crocs[i].x, crocs[i].x + CROC_WIDTH) && 
                    inBetween(frog.x, crocs[i].x, crocs[i].x + CROC_WIDTH)) ||
                    (inBetween(frog.x + FROG_WIDTH, crocs[i].x, crocs[i].x + CROC_WIDTH) && 
                    inBetween(frog.x + FROG_WIDTH, crocs[i].x, crocs[i].x + CROC_WIDTH))
                )
            ) {
                frog_on_crocodile = i; // La rana è sopra il coccodrillo
                break;
            } else {
                frog_on_crocodile = -1; // La rana non è sopra nessun coccodrillo
            }
        
        }
       
     

        // Verifica se la rana è entrata in una tana
        int tanaIndex = isFrogInTana(&frog);
        if (tanaIndex != -1) {
            taneOccupate[tanaIndex] = 1; 
            tanaIndex = -1; // Reset dell'indice della tana
        }

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

        
        drawCrocodiles(crocs, totalCrocs); // Disegna i coccodrilli
        drawVoid();
        drawFrog(&frog);
        drawTane(taneOccupate);


        // Disegno dei bullet
        for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
            if (bullets[i].inGioco) {
                drawBullet(&bullets[i]);  // Usa drawBullet
            }
        }
        // Disegno delle granate
        for (int i = 0; i < MAX_GRENADES; i++) {  // Usa MAX_BULLETS
            if (grenades[i].inGioco) {
                drawGrenade(&grenades[i]);  // Usa drawBullet
            }
        }
        refresh();

    }



    endwin();
    kill(pid_frog, SIGTERM);
    for (int i = 0; i < totalCrocs; i++){
        kill(pid_crocs[i], SIGTERM); // Termina i processi dei coccodrilli
    }
    wait(NULL);
    wait(NULL);
    return 0;
}

