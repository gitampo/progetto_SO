#ifndef CROC_H
#define CROC_H

#include "entity.h"

// Dimensioni dei coccodrilli
#define COCC_WIDTH  6
#define COCC_HEIGHT 3

// La funzione che gestisce i coccodrilli nel processo figlio
void runCrocs(Entity crocs[], int numCrocs, int pipeFD);

#endif // CROC_H
