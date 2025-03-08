#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"

#ifndef CROC_H
#define CROC_H

#define SPRITE_CROC 'C'

void croc(Crocodile *croc, int fileds[2]);

#endif // CROC_H