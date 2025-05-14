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

void* bulletThread( void *arg) {
    bullet_args *args = (bullet_args *)arg;
    Entity *bullet = &args->bullet;
    entity_buffer *fileds = args->fileds;

    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;

    int stop_update = 0; // Flag per fermare l'aggiornamento della posizione

    while (1) {
       
        bullet->x += bullet->direction * bullet->speed;
        // Controlla se il proiettile ha superato i bordi
        if (!stop_update && bullet->x >= endCol || bullet->x < startCol) {
            bullet->inGioco = 0;
            stop_update = 1; // Ferma l'aggiornamento della posizione
            write_from_buffer(fileds, bullet);
        }

        if (bullet->inGioco)
            write_from_buffer(fileds, bullet);
        usleep(25000);  // Più veloce dei coccodrilli
    }
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

void* grenadeThread(void *arg) {
    grenade_args *args = (grenade_args *)arg;
    Entity *grenade = &args->grenade;
    entity_buffer *fileds = args->fileds;


    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;

    int stop_update = 0; // Flag per fermare l'aggiornamento della posizione

    while (1) {
        grenade->x += grenade->direction * grenade->speed;
        // Controlla se il proiettile ha superato i bordi
        if (!stop_update && grenade->x >= endCol || grenade->x < startCol) {
            grenade->inGioco = 0;
            stop_update = 1; // Ferma l'aggiornamento della posizione
            write_from_buffer(fileds, grenade);
        }

        if (grenade->inGioco)
            write_from_buffer(fileds, grenade);
        
        usleep(25000);  // Più veloce dei coccodrilli
    }
}