#ifndef GRENADE_H
#define GRENADE_H

#define SYMBOL_GRENADE 'G'

typedef struct {
    int id;
    int x, y;
    int width, height;
    int active;
    int dir;  // Direzione di movimento (-1 sinistra, 1 destra)
} GrenadeData;

// Funzione per far muovere la granata come processo separato
void grenade(GrenadeData* g, int pipeWrite);

#endif // GRENADE_H
