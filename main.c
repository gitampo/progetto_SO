#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"
#include "frog.h"

#define SPRITE_FROG "AAA"
#define SPRITE_CROC "CCC"

int main (){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    box(stdscr, ACS_VLINE, ACS_HLINE);

    Coordinates frog1 = {1, LINES - 2, COLS - 2}; // Modificato da puntatore a variabile
    Coordinates croc1 = {2, 1, 1}; // Modificato da puntatore a variabile
    Coordinates temp;

    mvaddch(frog1.y, frog1.x, SPRITE_FROG);
    mvaddch(croc1.y, croc1.x, SPRITE_CROC);
    refresh();

    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore fileds");
        endwin();
        exit(-1);
    }

    pid_t pid_frog = fork();
    if (pid_frog == 0) {
        close(fileds[0]);
        frog(&frog1, fileds); // Passa la variabile direttamente
        exit(0);
    }

    pid_t pid_croc = fork();
    if (pid_croc == 0) {
        close(fileds[0]);
        croc(&croc1, fileds); // Passa la variabile direttamente
        exit(0);
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
            } else if (temp.id == 2) { // Aggiorna la posizione del COCCODRILLO
                mvaddch(croc1.y, croc1.x, ' ');
                croc1.y = temp.y;
                croc1.x = temp.x;
                mvaddch(croc1.y, croc1.x, SPRITE_CROC);
            }

            refresh();
            usleep(50000); // Ritardo per evitare un loop troppo veloce
        }

}  

endwin();
kill(pid_frog, SIGKILL);
kill(pid_croc, SIGKILL);
wait(NULL);
return 0;
}