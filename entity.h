#ifndef ENTITY_H
#define ENTITY_H

#include <sys/types.h>

typedef enum {
    OBJECT_FROG,
    OBJECT_CROCODILE,
    OBJECT_BULLET,
    OBJECT_GRENADE,  // Aggiungi questo tipo per la granata
} ObjectType;

#define FUORI -1000  // Valore speciale per indicare che l'oggetto è fuori

typedef struct {
    int id;
    ObjectType type;
    int attached; // 0 se non attaccato, 1 se attaccato
    int attached_crocodile_id; // ID del coccodrillo a cui è attaccato
    int x;
    int y;
    int direction;
    int initX;
    int inGioco;
    int speed;             // Se usato per altri scopi
    int collisionCooldown; // 0 se la collisione è attiva, >0 se in cooldown
    pid_t pid;  
} Entity;

#endif // ENTITY_H
