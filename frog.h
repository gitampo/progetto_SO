#ifndef FROG_H
#define FROG_H

// Simboli ASCII (gamelogic.c li userà per disegnare la rana)
#define SYMBOL_FROG_1 " @..@"
#define SYMBOL_FROG_2 "(----)"
#define SYMBOL_FROG_3 " ^^ ^^"

/*
 * Struttura della Rana
 *  - id = 1
 *  - (x,y): coord correnti
 *  - width/height: dimensioni bounding box
 *  - lives: vite
 */
typedef struct {
    int id;     
    int x, y;   
    int width;
    int height;
    int lives;
} FrogData;

// Funzione che il figlio "rana" esegue dopo il fork
// -> legge i tasti con getch(), aggiorna (x,y) e scrive su pipe
void frogProcess(FrogData* frog, int pipefd[2]);

#endif // FROG_H
