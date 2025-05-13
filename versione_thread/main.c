#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <string.h> 
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
#include <pthread.h>

extern pthread_mutex_t mutex; // Dichiarazione del mutex

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    setlocale(LC_ALL, "");

    srand(time(NULL)); 
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_YELLOW);
    init_pair(4, COLOR_GREEN, COLOR_BLUE);
    if (can_change_color()) {
        init_color(COLOR_YELLOW, 500, 250, 0);
    }
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_RED, COLOR_BLACK);
    init_pair(9, COLOR_WHITE, COLOR_WHITE);

    nodelay(stdscr, TRUE);

    pthread_t thread_frog;
    pthread_t thread_crocs[totalCrocs];  // Array per memorizzare i pid dei coccodrilli
    pthread_t thread_bullet;
    pthread_t thread_grenade;

    int flags = fcntl(fileds[0], F_GETFL, 0);
    int frogflags = fcntl(toFrog[0], F_GETFL, 0);
    int lives = 3;  
    int game_win = 0; // Flag per indicare se il gioco è vinto
    int holeRow     = HOLE_ROW;               // macro già in graphics.h
    int labelLen    = strlen("Lives:");
    int heartsStart = startCol + labelLen + 1;
    int timerCol    = heartsStart + 3*2 + 2;   // tre cuori * 2 colonne + gap
    int taneRow  = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;
    int heartRow = taneRow - 1;
    int maxTime = 60; // Tempo massimo in secondi
    int bar_timeLeft = maxTime; // Tempo rimanente
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    int riverStartRow = LINES - 27;
    int totalGrenades = 20;
    int i_last_grenades = 0; // Indice dell'ultimo proiettile creato
    int i_last_bullet = 0; // Indice dell'ultimo proiettile creato
    int crocodileSpeed = 0;
    int validX_min = (COLS - PAVEMENT_WIDTH) / 2;
    int validX_max = validX_min + PAVEMENT_WIDTH - FROG_WIDTH;
    int validY_min = LINES - 33;
    int validY_max = LINES - FROG_HEIGHT;
    int frog_on_crocodile = -1;
    int taneOccupate[NUM_TANE] = {0};  // 0 = libera, 1 = occupata
    int totalCrocs = LANES * CROCS_PER_LANE; // 16 Coccodrilli totali
   
    time_t bar_lastTick = time(NULL);
      
    Entity frog;
    frog.type = OBJECT_FROG;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;

    Entity crocs[totalCrocs];
    Entity bullets[MAX_BULLETS];  // Usa MAX_BULLETS invece di MAX_PROJECTILES
    Entity grenades[MAX_GRENADES];  // Usa MAX_BULLETS invece di MAX_PROJECTILES
    Entity temp;

    fcntl(fileds[0], F_SETFL, O_NONBLOCK);
    fcntl(toFrog[0], F_SETFL, O_NONBLOCK);

    // Calcola i limiti e inizializza l'area del fiume

    pthread_create(&thread_frog, NULL, frogProcess, &frog);
    // Fork per la rana
    creaCrocodiles(crocs, startCol, endCol, riverStartRow);
    pthread_create(&thread_crocs[i], NULL, crocThread, &crocs[i]);

    // Inizializzazione dell'array per i bullet 
    for (int i = 0; i < MAX_BULLETS; i++) {  // Usa MAX_BULLETS
        bullets[i].inGioco = 0;
    }

    // Inizializzazione dell'array per le granate  
    for (int i = 0; i < MAX_GRENADES; i++) {  // Usa MAX_BULLETS
        grenades[i].inGioco = 0;
    }

    while (game_win == 0 && lives > 0) {

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
                
               pthread_create(&pid_bullets[i_last_bullet], NULL, bulletThread, &bullets[i_last_bullet]);

                i_last_bullet = (i_last_bullet + 1) % MAX_BULLETS; // Incrementa l'indice dell'ultimo proiettile creato

            } else if (temp.type == OBJECT_BULLET) {
                // exit(EXIT_SUCCESS); // Termina il processo figlio del proiettile
                if (temp.inGioco) {
                    bullets[temp.id] = temp; // Aggiungi il proiettile all'array
                } else {
                    // Rimuovi il proiettile dall'array
                    bullets[temp.id].inGioco = 0;
                    kill(pid_bullets[temp.id], SIGKILL);
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
                    temp.speed = 1; // Imposta la velocità della granata
                    temp.direction = grenade_direction[i]; // Imposta la direzione della granata
                    grenades[i_last_grenades] = temp; // Aggiungi il proiettile all'array
                    pthread_create(&pid_grenades[i_last_grenades], NULL, grenadeThread, &grenades[i_last_grenades]);

                    i_last_grenades = (i_last_grenades + 1) % MAX_GRENADES; // Incrementa l'indice dell'ultimo proiettile creato
                }

            } else if (temp.type == OBJECT_GRENADE) {
                // exit(EXIT_SUCCESS); // Termina il processo figlio del proiettile
                if (temp.inGioco) {
                    grenades[temp.id] = temp; // Aggiungi il proiettile all'array
                } else {
                    // Rimuovi il proiettile dall'array
                    grenades[temp.id].inGioco = 0;
                    
                }

            }else if(temp.type == PAUSE_GAME){
                bool is_paused = 1;
                
                for (int i = 0; i < totalCrocs; i++) kill(pid_crocs[i], SIGSTOP);
                for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i].inGioco) kill(pid_bullets[i], SIGSTOP);
                for (int i = 0; i < MAX_GRENADES; i++) if (grenades[i].inGioco) kill(pid_grenades[i], SIGSTOP);
                clear();
                mvprintw(LINES/2,   (COLS-9)/2,  "Gioco in pausa.");
                mvprintw(LINES/2+1, (COLS-20)/2, "Premi 'p' per continuare.");
                refresh();
                nodelay(stdscr, FALSE); // Disabilita la modalità non bloccante

             while (is_paused) {
                if(getch() == 'p') {
                    is_paused = 0;
                    
                    for (int i = 0; i < totalCrocs; i++) kill(pid_crocs[i], SIGCONT);
                    for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i].inGioco) kill(pid_bullets[i], SIGCONT);
                    for (int i = 0; i < MAX_GRENADES; i++) if (grenades[i].inGioco) kill(pid_grenades[i], SIGCONT);
                }
            }
        }
        }
        time_t now = time(NULL);
        if (now - bar_lastTick >= 1) {
            bar_lastTick = now;
            bar_timeLeft--;
            if (bar_timeLeft <= 0) {
                // tempo scaduto: perdi vita e reset rana
                frog.y = LINES - FROG_HEIGHT; 
                frog.x = (COLS - FROG_WIDTH) / 2;
                
                lives--;
                 if (lives <= 0) {
                    
                    break;
                 }
                bar_timeLeft = maxTime;
            }
        }
   
        if (frog.y >= riverStartRow && frog.y < riverStartRow + RIVER_HEIGHT) {
            for(int i = 0; i < totalCrocs; i++) {
                if(
                    (frog.y == crocs[i].y) && 
                    (
                        (inBetween(frog.x, crocs[i].x + 3, crocs[i].x + CROC_WIDTH - 3) ||
                        inBetween(frog.x + FROG_WIDTH, crocs[i].x + 3, crocs[i].x + CROC_WIDTH - 3))
                    )
                ) {
                    frog_on_crocodile = i; // La rana è sopra il coccodrillo
                    break;
                } else {
                    frog_on_crocodile = -1; // La rana non è sopra nessun coccodrillo
                }
            }

            if (frog_on_crocodile == -1) {
                //in futuro devi decrementare la vita della rana
                frog.y = LINES - FROG_HEIGHT; 
                frog.x = (COLS - FROG_WIDTH) / 2;
        
                lives--;
                bar_timeLeft = maxTime;
                 if (lives <= 0) {
                    break;
                 }
            }

        } else {
            frog_on_crocodile = -1; // La rana non è sopra nessun coccodrillo
        }

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!bullets[i].inGioco) {
                continue; // Se il proiettile non è in gioco, salta l'iterazione
            }
           if (bullets[i].y == frog.y + FROG_HEIGHT / 2 && inBetween(bullets[i].x, frog.x, frog.x + FROG_WIDTH - 1)) {
                // La rana è colpita da un proiettile
                frog.y = LINES - FROG_HEIGHT; 
                frog.x = (COLS - FROG_WIDTH) / 2;
                write(toFrog[1], &frog, sizeof(Entity)); // Invia la rana al processo padre
                bullets[i].inGioco = 0; // Rimuovi il proiettile
                kill(pid_bullets[i], SIGKILL); // Termina il processo del proiettile
                lives--;
                bar_timeLeft = maxTime;
               break;
            }
        }

        if (lives <= 0) {
            
            break;
         }

        for (int i = 0; i < MAX_BULLETS; i++){
            if (!bullets[i].inGioco) {
                continue; // Se il proiettile non è in gioco, salta l'iterazione
            } for (int j = 0; j < MAX_GRENADES; j++)
            {
                if (!grenades[j].inGioco) {
                    continue; // Se il proiettile non è in gioco, salta l'iterazione
                }
                if (bullets[i].y == grenades[j].y && bullets[i].x == grenades[j].x) {
                    bullets[i].inGioco = 0; // Rimuovi il proiettile
                    kill(pid_bullets[i], SIGKILL); // Termina il processo del proiettile
                    grenades[j].inGioco = 0; // Rimuovi la granata
                    kill(pid_grenades[j], SIGKILL); // Termina il processo della granata
                    break;
                }
            }
        
        }


        // calcolo la riga delle tane
int holeRow = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;

// ho raggiunto l’area delle tane?
if (frog.y == holeRow) {
    int tanaIndex = isFrogInTana(&frog);
    if (tanaIndex != -1) {
        // è una tana valida
        if (!taneOccupate[tanaIndex]) {
            // occupo la tana libera
            taneOccupate[tanaIndex] = 1;
            bar_timeLeft = maxTime;
        } else {
            // tana già piena → perdi una vita e resetto
            lives--;
            bar_timeLeft = maxTime;
            if (lives <= 0) {
                break;
            }
            frog.y = LINES - FROG_HEIGHT;
            frog.x = (COLS - FROG_WIDTH)/2;
            continue;
        }
    } else {
        // sei sopra la riga delle tane ma non in una tana → perdi vita
        lives--;
        bar_timeLeft = maxTime;
        if (lives <= 0) {
            break;
        }
        frog.y = LINES - FROG_HEIGHT;
        frog.x = (COLS - FROG_WIDTH)/2;
        continue;
    }

    // controllo vittoria
    int allFilled = 1;
    for (int ti = 0; ti < NUM_TANE; ti++) {
        if (!taneOccupate[ti]) {
            allFilled = 0;
            break;
        }
    }
    if (allFilled && lives > 0) {
        game_win = 1;
        break;
    }
}

    erase();

    // ricalcola la riga delle tane e quella dei cuori
    int taneRow  = ((LINES - 27) - PAVEMENT_HEIGHT) - TANE_HEIGHT;
    int heartRow = taneRow - 1;

    // 1) stampa il label "Lives:" a sinistra
    mvprintw(heartRow, startCol, "Lives:");

    // 2) a destra di "Lives:" lascia uno spazio, poi disegna i cuori
    // int heartsStart = startCol + strlen("Lives:") + 1;
    for (int i = 0; i < 3; i++) {
        int col = heartsStart + i * 2;
        if (i < lives) {
            attron(COLOR_PAIR(8));
            mvprintw(heartRow, col, "%lc", L'♥');
            attroff(COLOR_PAIR(8));
        } else {
            mvaddch(heartRow, col, ' ');
        }
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

        drawTimer(bar_timeLeft, maxTime, heartRow, timerCol); // Disegna il timer

        refresh();

    }
      

    if (game_win) {
        // Schermata vittoria
       
        clear();
        mvprintw(LINES/2,   (COLS-12)/2, "HAI VINTO!");
        mvprintw(LINES/2+1, (COLS-28)/2, "Premi un tasto per uscire");
        refresh();
        nodelay(stdscr, FALSE);

        getch();
        endwin();
    } else {
        // Schermata sconfitta (già c’era)
       
        clear();
        mvprintw(LINES/2,   (COLS-9)/2,  "GAME OVER");
        mvprintw(LINES/2+1, (COLS-21)/2, "Premi un tasto per uscire");
        refresh();
        nodelay(stdscr, FALSE);

        getch();
        endwin();
    }

    
    wait(NULL);
    wait(NULL);
    return 0;
}

