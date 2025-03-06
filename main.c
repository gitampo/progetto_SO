#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"
#include "frog.h"


#define create_process(pid, callback) { \
    pid_t pid = fork(); \
    if (pid < 0) { \
        perror("Errore fork"); \
        endwin(); \
        exit(1); \
    } if (pid == 0) { \
        callback(); \
        endwin(); \
        return 0; \
    } \
}


int main (){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    create_process(pidFrog, frog);
    create_process(pidCroc, croc);
    create_process(pidPipe, pipe);

    Coordinates *frog1 = {1, LINES - 1, COLS/2};
    Coordinates *croc1 = {2, LINES - 1, COLS};

    

    pid_t pidFrog = fork();





}