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

#define PROJECTILE_SPEED 5

// Il bullet si muove in orizzontale e viene disegnato come un '*'.
// Quando esce dallo schermo, il processo termina.
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "entity.h"
#include "graphics.h"
#include <ncurses.h>
#include <string.h>

void bulletProcess(Entity *bullet, int pipeFD) {
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;

    while (1) {
       bullet->x += bullet->direction * bullet->speed;  // Muovi la granata
        //bullet->x += bullet->direction;  // Muovi la granata
        // Verifica se la granata ha superato il fiume (endCol)
        if (bullet->x >= endCol || bullet->x < startCol) {
            bullet->inGioco = 0;  // Imposta lo stato della granata come inattiva
            write(pipeFD, bullet, sizeof(Entity));  // Invia al padre per rimuovere la granata

            close(pipeFD);

            // Controllo che il pid esista prima di tentare di terminare il processo
            if (bullet->pid > 0) {
                kill(bullet->pid, SIGTERM);  // Termina il processo figlio
                waitpid(bullet->pid, NULL, WNOHANG);  // Non bloccare l'esecuzione
            }

            _exit(0);  // Termina il processo figlio
        }

        // Scrivi la posizione aggiornata della granata sulla pipe
        write(pipeFD, bullet, sizeof(Entity));
        usleep(80000);  // Delay per il movimento
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
