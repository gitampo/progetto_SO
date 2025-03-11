#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SPRITE_CROC 'C'
#define PAVEMENT_HEIGHT 3
#define PAVEMENT_WIDTH  84 // Larghezza del marciapiede

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
void drawPavement();
void drawRiver();

#endif // GRAPHICS_H
