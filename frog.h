#ifndef FROG_H 
#define FROG_H 
 
#include "entity.h" 
 
// Dimensioni della rana (usate anche per il disegno e i limiti) 
#define FROG_WIDTH  8 
#define FROG_HEIGHT 3 
 
// Definisci le stringhe per lo sprite della rana 
#define SYMBOL_FROG_1 "  @..@  " 
#define SYMBOL_FROG_2 " (----) " 
#define SYMBOL_FROG_3 "  ~~~~  " 
 
// Funzioni per la gestione della rana 
void drawFrog(const Entity *frog); 
void clearFrog(const Entity *frog); 
void frogProcess(Entity *frog, int pipeFD[2]); 
 
#endif // FROG_H 