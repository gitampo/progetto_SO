#ifndef CROC_H
#define CROC_H

#include "graphics.h"

// Numero di coccodrilli (lo usiamo anche nel figlio).
#define NUM_CROCS 5

// Funzione chiamata dal processo figlio, che gestisce TUTTI i coccodrilli.
// Legge/scrive su writeFD per comunicare le posizioni al processo padre.
void runCrocs(Crocodile crocs[], int numCrocs, int writeFD);

#endif // CROC_H
