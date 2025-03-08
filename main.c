#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"
#include "frog.h"
#include "croc.h"

#define NUM_CROCS 5

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    box(stdscr, ACS_VLINE, ACS_HLINE);

    Coordinates frog1 = {1, LINES - 2, COLS - 2}; // Modificato da puntatore a variabile
    Crocodile croc1[NUM_CROCS] = {
        {2, {7, 2}},
        {3, {2, 4}},
        {4, {6, 6}},
        {5, {7, 8}},
        {6, {8, 7}}
    };
    Coordinates temp;

    mvaddch(frog1.y, frog1.x, SPRITE_FROG);
    for (int i = 0; i < NUM_CROCS; i++) {
        mvaddch(croc1[i].coords.y, croc1[i].coords.x, SPRITE_CROC);
    }
    refresh();

    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore fileds");
        endwin();
        exit(-1);
    }

    pid_t pid_frog = fork();
    if (pid_frog == -1) {
        perror("Errore fork frog");
        endwin();
        exit(-1);
    } else if (pid_frog == 0) {
        close(fileds[0]);
        frog(&frog1, fileds);
        exit(0);
    }
    pid_t pid_crocs[NUM_CROCS]; // Array per memorizzare i PID dei coccodrilli

    for (int i = 0; i < NUM_CROCS; i++) {
        pid_t pid_croc = fork();
        if (pid_croc == -1) {
            perror("Errore fork croc");
            endwin();
            exit(-1);
        } else if (pid_croc == 0) {
            close(fileds[0]);
            croc(&croc1[i], fileds);
            exit(0);
        } else {
            pid_crocs[i] = pid_croc; // Memorizza il PID del processo coccodrillo
        }
    }

    close(fileds[1]);

    while (1) {
        // Legge le coordinate aggiornate
        if (read(fileds[0], &temp, sizeof(Coordinates)) > 0) {
            if (temp.id == 1) { // Aggiorna la posizione della RANA
                mvaddch(frog1.y, frog1.x, ' ');
                frog1.y = temp.y;
                frog1.x = temp.x;
                mvaddch(frog1.y, frog1.x, SPRITE_FROG);
            } else { // Aggiorna la posizione del COCCODRILLO
                for (int i = 0; i < NUM_CROCS; i++) {
                    if (croc1[i].coords.id == temp.id) {
                        mvaddch(croc1[i].coords.y, croc1[i].coords.x, ' ');
                        croc1[i].coords.y = temp.y;
                        croc1[i].coords.x = temp.x;
                        mvaddch(croc1[i].coords.y, croc1[i].coords.x, SPRITE_CROC);
                    }
                }
            }
            refresh();
            usleep(100000);
        } else {
            perror("Errore read");
        }
    }

    endwin();
    kill(pid_frog, SIGKILL);
    for (int i = 0; i < NUM_CROCS; i++) {
        kill(pid_crocs[i], SIGKILL); // Termina i processi dei coccodrilli
    }
    wait(NULL);
    wait(NULL);
    return 0;
}