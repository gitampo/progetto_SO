// coccodrillo.h
#ifndef COCCODRILLO_H
#define COCCODRILLO_H

#include "config.h"

typedef struct {
    int id; 
    int x, y;
    int width, height;
    int dir;   
    int alive; 
    int oldX; // Se manca questo, avrai l’errore
} CrocData;

void generatoreCoccodrilli(int flussoY, int dir, int speed, int pipeWrite);

#endif
