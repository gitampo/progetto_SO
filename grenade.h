#ifndef GRENADE_H
#define GRENADE_H

#include <stdbool.h>

// Struttura per una granata
typedef struct {
    int x, y; // Posizione della granata
    bool active; // Stato della granata (attiva/inattiva)
    int direction; // Direzione: -1 (sinistra), 1 (destra)
} Grenade;

// Costanti
#define GRENADE_SYMBOL '*'

// Funzioni per la gestione delle granate
void initGrenade(Grenade *grenade, int x, int y, int direction);
void moveGrenade(int write_pipe, Grenade *grenade);

#endif // GRENADE_H
