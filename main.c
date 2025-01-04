#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <curses.h>
#include "frog.h"
#include "croc.h"
#include "graphics.h"

#define SPRITE_FROG 'A'
#define SPRITE_CROC 'CCCCCCCCCCC'

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
    Coordinate croc1 = {2, COLS, COLS};                 // ID 2 per il coccodrillo
    Coordinate temp;

    mvaddch(frog1.y, frog1.x, SPRITE_FROG);
    mvaddch(croc1.y, croc1.x, SPRITE_CROC);
    refresh();
    

    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore pipe");
        endwin();
        exit(EXIT_FAILURE);
    }

    pid_t pid_frog = fork();
    if (pid_frog == 0) {
        close(fileds[0]);
        frog(&frog1, fileds);
        exit(0);
    }

    pid_t pid_croc = fork();
    if (pid_croc == 0) {
        close(fileds[0]);
        croc(&croc1, fileds);
        exit(0);
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
                mvaddch(croc1.y, croc1.x, ' ');
                croc1.y = temp.y;
                croc1.x = temp.x;
                mvaddch(croc1.y, croc1.x, SPRITE_CROC);
            }
        }

        // Controllo collisione (commentato perche non c'è collisione tra rana e coccodrilli ma serve comunque dopo)
        /*if (frog1.y == croc1.y && frog1.x == croc1.x) {
            refresh();
            sleep(3);
            break;
        }*/

        refresh();
        usleep(50000); // Ritardo per evitare un loop troppo veloce
    }

    endwin();
    kill(pid_frog, SIGKILL);
    kill(pid_croc, SIGKILL);
    wait(NULL);
    wait(NULL);
    return 0;
}