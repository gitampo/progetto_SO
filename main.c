#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <curses.h>
#include "frog.h"
#include "croc.h"
#include "graphics.h"

#define SPRITE_FROG 'F'
#define SPRITE_CROC 'CCCCCCCCCCC'
#define NUM_CROCS 4
#define NUM_HOLES 3  // Numero di tane
#define SPRITE_HOLE 'TTTTTTTTTTTTTTTTT'  // Simbolo delle tane

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    //nodelay(stdscr, TRUE);
    box(stdscr, ACS_VLINE, ACS_HLINE);

    // Inizializza le coordinate con ID

    Coordinate frog1 = {1, LINES - 2, COLS / 2}; // ID 1 per la rana
    Coordinate croc1 = {2, LINES  - 10, COLS - 10}; // ID 2 per il coccodrillo
    Coordinate temp;

    Coordinate crocs[NUM_CROCS];

    for (int i = 0; i < NUM_CROCS; i++) {
    crocs[i].id = i + 2;  // ID da 2 in poi (1 è per la rana)
    crocs[i].y = LINES - 2;  // Posizione Y fissa
    crocs[i].x = COLS / (NUM_CROCS + 1) * i;  // Posizioni iniziali distribuite
    }

    Coordinate holes[NUM_HOLES];  // Array di tane

// Inizializza le tane nella parte superiore dello schermo
    for (int i = 0; i < NUM_HOLES; i++) {
    holes[i].id = i + 10;  // ID arbitrario (diverso da rana e coccodrilli)
    holes[i].y = 1;  // Riga in alto dello schermo
    holes[i].x = COLS / (NUM_HOLES + 1) * (i + 1);  // Posizione distribuita
    mvaddch(holes[i].y, holes[i].x, SPRITE_HOLE);
    }

// Controlla se la rana è entrata in una tana
    for (int i = 0; i < NUM_HOLES; i++) {
    if (frog1.y == holes[i].y && frog1.x == holes[i].x) {
        mvaddch(frog1.y, frog1.x, ' ');  // Cancella la rana
        mvprintw(LINES / 2, COLS / 2 - 5, "VITTORIA!");
        refresh();
        sleep(2);
        endwin();
        exit(0);
    }
}


    mvaddch(frog1.y, frog1.x, SPRITE_FROG);
    for (int i = 0; i < NUM_CROCS; i++) {
    mvaddch(crocs[i].y, crocs[i].x, SPRITE_CROC);
    }
    refresh();
    

    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore pipe");
        endwin();
        exit(-1);
    }

    pid_t pid_frog = fork();
    if (pid_frog == 0) {
        close(fileds[0]);
        frog(&frog1, fileds);
        exit(0);
    }

    
    pid_t croc_pids[NUM_CROCS];  // Array per memorizzare i PID dei coccodrilli

    for (int i = 0; i < NUM_CROCS; i++) {
        pid_t pid_croc = fork();
        if (pid_croc == 0) {
            close(fileds[0]);
            croc(&crocs[i], fileds);
            exit(0);
        } else {
            croc_pids[i] = pid_croc;  // Memorizza il PID del processo figlio
    } 
    }

    close(fileds[1]);


 while (1) {

        // Legge le coordinate aggiornate
        if (read(fileds[0], &temp, sizeof(Coordinate)) > 0) {
            if (temp.id == 1) { // Aggiorna la posizione della rana
                mvaddch(frog1.y, frog1.x, ' ');
                frog1.y = temp.y;
                frog1.x = temp.x;
                mvaddch(frog1.y, frog1.x, SPRITE_FROG);
            } else if (temp.id == 2) { // Aggiorna la posizione del coccodrillo
            for (int i = 0; i < NUM_CROCS; i++) {
                mvaddch(crocs[i].y, crocs[i].x, ' ');
                crocs[i].y = temp.y;
                crocs[i].x = temp.x;
                mvaddch(crocs[i].y, crocs[i].x, SPRITE_CROC);
            }
            }
        }


        // Controllo collisione (commentato perche non c'è collisione tra rana e coccodrilli ma serve comunque dopo)
        /*if (frog1.y == croc1.y && frog1.x == croc1.x) {
            refresh();
            sleep(3);
            break;
        }*/

        refresh();
        usleep(10000); // Ritardo per evitare un loop troppo veloce
    }

    endwin();
    kill(pid_frog, SIGKILL);
    kill(croc_pids, SIGKILL); 
    wait(NULL);
    wait(NULL);
    return 0;
}