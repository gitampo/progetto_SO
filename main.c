#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"

int main (){

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);


    Coordinates *frog1 = {1, LINES - 1, COLS/2};
    Coordinates *croc1 = {2, LINES - 1, COLS};

    void frog(Coordinates frog, int pipe[2]);

    pid_t pidFrog = fork();

    if (pidFrog < 0) {
    perror("Errore fork rana");
    endwin();
    exit(1);
    }

    if (pidFrog == 0) {
    frog(&frog1, pipe);
    exit(0);
    }

    
pid_t pidCroc = fork();
if(pidCroc < 0){
    perror("Errore fork coccodrillo");
    endwin();
    exit(1);
}

if(pidCroc == 0){
    croc(&croc, pipe);
    exit(0);
}

}