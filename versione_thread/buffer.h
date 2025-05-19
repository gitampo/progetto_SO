#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "entity.h"
#include <pthread.h>

//Buffer condiviso
#define BUFFER_SIZE 50 // Dimensione del buffer

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex per la sincronizzazione

typedef struct {
  Entity buffer[BUFFER_SIZE];
  int head;
  int tail;
  pthread_mutex_t mutex; // Mutex per la sincronizzazione
} entity_buffer;

typedef struct frog_args {
    Entity frog;
    entity_buffer* fileds;
    entity_buffer* toFrog;
} frog_args;

typedef struct croc_args {
    Entity croc;
    entity_buffer* fileds;
    int riverStart;
} croc_args;

typedef struct bullet_args {
    Entity bullet;
    entity_buffer* fileds;
} bullet_args;

typedef struct grenade_args {
    Entity grenade;
    entity_buffer* fileds;
} grenade_args;


entity_buffer* create_buffer();
void destroy_buffer(entity_buffer* buffer);
bool read_from_buffer(entity_buffer* buffer, Entity* dest);
bool write_from_buffer(entity_buffer* buffer, Entity* dest);

#endif // BUFFER_H