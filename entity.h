#ifndef ENTITY_H
#define ENTITY_H

#include <sys/types.h>

typedef enum {
    OBJECT_FROG,
    OBJECT_CROCODILE,
    OBJECT_BULLET,
    OBJECT_GRENADE,  // Aggiungi questo tipo per la granata
    FROG_ON_CROCODILE,
    CREATE_BULLET,
    CREATE_GRENADE
} ObjectType;

#define FUORI -1000  // Valore speciale per indicare che l'oggetto è fuori

typedef struct {
    int id;
    ObjectType type;
    int attached; // 1 se la rana è attaccata a un coccodrillo, 0 altrimenti
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

/*else if (temp.type == CREATE_BULLET) {
    // Creazione del proiettile

    bullets[temp.id] = temp; // Aggiungi il proiettile all'array
    
    pid_bullets[temp.id] = fork();
    if (pid_bullets[temp.id] == -1) {
        perror("Errore fork Proiettile");
        endwin();
        exit(EXIT_FAILURE);
    } else if (pid_bullets[temp.id] == 0) {
        close(fileds[0]);  // Ogni processo figlio deve chiudere il lato di lettura
        bulletProcess(&bullets[temp.id], fileds[1]);  // Ogni figlio gestisce un coccodrillo
        exit(EXIT_SUCCESS);  // Ogni processo figlio termina dopo aver eseguito il proprio lavoro
    }
}
    */