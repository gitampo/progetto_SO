#ifndef FROG_H
#define FROG_H

/*
 * Struttura della Rana:
 *   id = 1
 *   (x,y): coord correnti
 *   width/height: dimensioni bounding box
 *   lives: vite
 */
typedef struct {
    int id;     
    int x, y;   
    int width;
    int height;
    int lives;
} FrogData;

// Funzione che il figlio "rana" esegue dopo il fork
void frogProcess(FrogData* frog, int pipefd[2]);

#endif // FROG_H
