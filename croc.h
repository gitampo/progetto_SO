#ifndef CROC_H
#define CROC_H

/*
 * Struttura del Coccodrillo:
 *   id = 2..N
 */
typedef struct {
    int id;
    int x, y;
    int oldX;
    int width, height;
    int alive;
} CrocData;

// Funzione che il figlio "coccodrillo" esegue dopo il fork
void crocProcess(CrocData* croc, int pipefd[2]);

#endif // CROC_H
