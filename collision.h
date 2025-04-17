#ifndef COLLISION_H
#define COLLISION_H

// Restituisce l'indice della tana in cui è la rana (-1 se non è in nessuna)
int isFrogInTana(const Entity *frog);
bool inBetween(int value, int min, int max);
void grenadeProcess(Entity *grenade, int pipeFD[2]);
bool collision(Entity frog, Entity tana);

#endif