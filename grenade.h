#ifndef GRENADE_H
#define GRENADE_H

#define SYMBOL_GRENADE 'G'

typedef struct {
    int id;
    int x, y;
    int width, height;
    int active;
} GrenadeData;

#endif // GRENADE_H
