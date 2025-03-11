#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include "graphics.h"

#ifndef FROG_H
#define FROG_H

#define SPRITE_FROG   "@       @"
#define SYMBOL_FROG_2 "(----)"
#define SYMBOL-FROG_3 " ^^^^"

void frog(Coordinates *frog, int fileds[2]);

#endif
