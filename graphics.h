#ifndef GRAPHICS_H
#define GRAPHICS_H

// Sprite per disegnare la rana e i coccodrilli
#define SPRITE_FROG '@'
#define SPRITE_CROC 'C'

// Struttura base con ID e coordinate
typedef struct {
    int id;
    int y;
    int x;
} Coordinates;

// Struttura che rappresenta un coccodrillo
typedef struct {
    int id;
    Coordinates coords;
    int direction; // +1 per muoversi a destra, -1 per muoversi a sinistra
} Crocodile;

#endif // GRAPHICS_H
