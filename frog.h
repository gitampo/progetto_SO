#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"

#ifndef FROG_H
#define FROG_H

void frog(Coordinates frog, int pipe[2]);

#endif
