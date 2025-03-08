#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define NUM_CROCS 5

typedef struct {
    int id; 
    int y;
    int x;
} Coordinates;

typedef struct {
    int id;
    Coordinates coords;
} Crocodile;


#endif 


