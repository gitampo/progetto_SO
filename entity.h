#ifndef ENTITY_H
#define ENTITY_H

typedef enum {
    OBJECT_FROG,
    OBJECT_CROCODILE
} ObjectType;

#define FUORI -1000  // valore speciale per indicare che l'oggetto è fuori dall'area

typedef struct {
    int id; // riconoscere oggeti che hanno lo stesso "type"
    ObjectType type;
    int x;
    int y;
    int direction; // per i Coccodrilli
    int initX;     // posizione iniziale per il respawn
    int inGioco;   // 1 se l'oggetto è in gioco, 0 se è "morto"
    int speed;     // velocità del movimento
} Entity;

#endif // ENTITY_H
