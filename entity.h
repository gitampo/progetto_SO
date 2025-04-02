#ifndef ENTITY_H
#define ENTITY_H

typedef enum {
    OBJECT_FROG,
    OBJECT_CROCODILE,
    CREATE_GRENADE,
    OBJECT_GRENADE
} ObjectType;



#define FUORI -1000  // Valore speciale per indicare che l'oggetto è fuori

typedef struct {
    int id;
    ObjectType type;
    int x;
    int y;
    int direction;
    int initX;
    int inGioco;
    int speed;             // Se usato per altri scopi
    int collisionCooldown; // 0 se la collisione è attiva, >0 se in cooldown
} Entity;

#endif // ENTITY_H
