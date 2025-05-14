#ifndef BULLET_H
#define BULLET_H

#include "buffer.h"
#include "entity.h"

#define MAX_BULLETS 100  // Usa un nome coerente con "bullet" al posto di "projectile"
#define MAX_GRENADES 100 // Usa un nome coerente con "grenade" al posto di "bomb"
#define BULLET_SPEED 2  // Velocità dei proiettili

// Dichiarazione delle funzioni
void* bulletThread(void* arg);  // Funzione per gestire il movimento del bullet

// Funzione per creare un bullet
void createBullet(Entity *bullet, int x, int y, int direction, int isGrenade);

// Funzione per disegnare il bullet
void drawBullet(Entity *bullet);

void* grenadeThread(void* arg);  // Funzione per gestire il movimento della granata

// Funzione per gestire la collisione tra il bullet e la rana
int collisionBullet(Entity *bullet, Entity *frog);

// Funzione per controllare se il bullet è in gioco
int isBulletInGame(Entity *bullet);

// Funzione per controllare se il bullet è uscito dallo schermo
int isBulletOutOfScreen(Entity *bullet);




#endif // BULLET_H
