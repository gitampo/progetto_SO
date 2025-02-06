#ifndef BULLET_H
#define BULLET_H

#define SYMBOL_BULLET '^'

typedef struct {
    int id;
    int x, y;
    int width, height;
    int active;
} BulletData;

#endif // BULLET_H
