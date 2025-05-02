#include "entity.h"
#include "graphics.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>  // Aggiungi questa libreria per usare la funzione kill
#include "bullet.h"
#include "collision.h"
#include "frog.h"
#include <fcntl.h>
#include <string.h>

#define PROJECTILE_SPEED 5

// Il bullet si muove in orizzontale e viene disegnato come un '*'.
// Quando esce dallo schermo, il processo termina.

void bulletProcess(Entity *bullet, int pipeFD) {
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;

    int stop_update = 0; // Flag per fermare l'aggiornamento della posizione

    while (1) {
        bullet->x += bullet->direction * bullet->speed;
        // Controlla se il proiettile ha superato i bordi
        if (!stop_update && bullet->x >= endCol || bullet->x < startCol) {
            bullet->inGioco = 0;
            stop_update = 1; // Ferma l'aggiornamento della posizione
            write(pipeFD, bullet, sizeof(Entity));
        }

        if (bullet->inGioco)
            write(pipeFD, bullet, sizeof(Entity));
        
        usleep(100000);  // Più veloce dei coccodrilli
    }
}

// Funzione per creare un bullet
void createBullet(Entity *bullet, int x, int y, int direction, int isGrenade) {
    bullet->x = x;
    bullet->y = y;
    bullet->direction = direction;
    bullet->type = isGrenade ? OBJECT_GRENADE : OBJECT_BULLET;  
    bullet->inGioco = 1;
    bullet->speed = isGrenade ? 2 : 1; // Granate sono più lente
}



// Funzione per gestire la collisione tra il bullet e la rana
int collisionBullet(Entity *bullet, Entity *frog) {
    if (bullet->inGioco && bullet->y == frog->y) {
        if (inBetween(bullet->x, frog->x, frog->x + FROG_WIDTH) ||
            inBetween(bullet->x + 1, frog->x, frog->x + FROG_WIDTH)) {
            return 1; // Collisione avvenuta
        }
    }
    return 0; // Nessuna collisione
}

// Funzione per controllare se il bullet è in gioco
int isBulletInGame(Entity *bullet) {
    return bullet->inGioco;
}

// Funzione per controllare se il bullet è uscito dallo schermo
int isBulletOutOfScreen(Entity *bullet) {
    return bullet->x < 0 || bullet->x >= COLS;
}

void grenadeProcess(Entity *grenade, int pipeFD) {
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;

    int stop_update = 0; // Flag per fermare l'aggiornamento della posizione

    while (1) {
        grenade->x += grenade->direction * grenade->speed;
        // Controlla se il proiettile ha superato i bordi
        if (!stop_update && grenade->x >= endCol || grenade->x < startCol) {
            grenade->inGioco = 0;
            stop_update = 1; // Ferma l'aggiornamento della posizione
            write(pipeFD, grenade, sizeof(Entity));
        }

        if (grenade->inGioco)
            write(pipeFD, grenade, sizeof(Entity));
        
        usleep(100000);  // Più veloce dei coccodrilli
    }
}