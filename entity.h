#ifndef ENTITY_H
#define ENTITY_H

// Tipo per distinguere i vari oggetti
typedef enum {
    OBJECT_FROG,
    OBJECT_CROCODILE
} ObjectType;

// Struttura unificata per ogni oggetto (rana, coccodrillo, ecc.)
typedef struct {
    int id;         // ID univoco
    ObjectType type; // Tipo dell'oggetto (es. OBJECT_FROG o OBJECT_CROCODILE)
    int x;          // Coordinata x
    int y;          // Coordinata y
} Entity;

#endif // ENTITY_H