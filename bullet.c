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


void bulletProcess(Entity *bullet, int fileds[2]) {
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;

    while (1) {
        bullet->x += bullet->direction * bullet->speed;  // Muovi il bullet

        // Verifica se il bullet ha superato il fiume (endCol)
        if (bullet->x >= endCol || bullet->x < startCol) {
            bullet->inGioco = 0;  // Imposta lo stato del proiettile come inattivo
            write(fileds[1], bullet, sizeof(Entity));  // Invia al padre per rimuovere il bullet

            close(fileds[1]);  // Chiudi la pipe

            // Controllo che il pid esista prima di tentare di terminare il processo
            if (bullet->pid > 0) {
                kill(bullet->pid, SIGTERM);  // Termina il processo figlio
                waitpid(bullet->pid, NULL, _NOCHANGE);  // Non bloccare l'esecuzione
            }

            _exit(0);  // Termina il processo figlio
        }

        // Scrivi la posizione aggiornata del bullet sulla pipe
        write(fileds[1], bullet, sizeof(Entity));
        usleep(80000);  // Delay per il movimento
    }
}




// Funzione per creare un bullet
void createBullet(Entity *bullet, int x, int y, int direction, int isGrenade) {
    bullet->x = x;
    bullet->y = y;
    bullet->direction = direction;
    bullet->type = isGrenade ? OBJECT_GRENADE : OBJECT_BULLET;  // Usa OBJECT_PROJECTILE
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

void grenadeProcess(Entity *grenade, int fileds[2]) {
    while (1) {
        grenade->x += grenade->direction * grenade->speed; // Muovi la granata
        usleep(100000); // Aspetta 0.1 secondi
        write(fileds[1], grenade, sizeof(Entity)); // Invia la posizione della granata al padre
    }
}