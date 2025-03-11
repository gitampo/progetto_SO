#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SPRITE_CROC 'C'
#define PAVEMENT_HEIGHT 3
#define PAVEMENT_WIDTH  80

// Altezza fiume
#define RIVER_HEIGHT 24

typedef struct {
    int id;
    int y;
    int x;
} Coordinates;

typedef struct {
    int id;
    Coordinates coords;
    int direction;
} Crocodile;

// Funzioni di disegno
void drawPavementCentered();
void drawRiver();
void drawPavement();

#endif // GRAPHICS_H
