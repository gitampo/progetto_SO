#ifndef COLLISION_H
#define COLLISION_H

#include "frog.h"
#include "graphics.h"
#include "entity.h"

// Restituisce l'indice della tana in cui è la rana (-1 se non è in nessuna)
int isFrogInTana(const Entity *frog);

#endif