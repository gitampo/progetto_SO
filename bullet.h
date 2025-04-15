#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

#define MAX_BULLETS 100  // Usa un nome coerente con "bullet" al posto di "projectile"

// Dichiarazione delle funzioni
void bulletProcess(Entity *bullet, int pipeFD);  // Funzione per gestire il movimento del bullet

// Funzione per creare un bullet
void createBullet(Entity *bullet, int x, int y, int direction, int isGrenade);

// Funzione per disegnare il bullet
void drawBullet(Entity *bullet);

// Funzione per gestire la collisione tra il bullet e la rana
int collisionBullet(Entity *bullet, Entity *frog);

// Funzione per controllare se il bullet è in gioco
int isBulletInGame(Entity *bullet);

// Funzione per controllare se il bullet è uscito dallo schermo
int isBulletOutOfScreen(Entity *bullet);




#endif // BULLET_H
