#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <fcntl.h> // Per O_NONBLOCK
#include "graphics.h"
#include "frog.h"
#include "croc.h"
#include "entity.h"  
#include "collision.h"
#include "bullet.h"
#include <pthread.h>

//Buffer condiviso
#define BUFFER_SIZE 10 // Dimensione del buffer

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex per la sincronizzazione
int buffer[BUFFER_SIZE]; // Buffer condiviso
int in = 0;   // posizione di scrittura
int out = 0;  // posizione di lettura

void write_to_buffer(int value) {
    pthread_mutex_lock(&buffer_mutex);
    buffer[in] = value;
    in = (in + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&buffer_mutex);
}

int read_from_buffer() {
    pthread_mutex_lock(&buffer_mutex);
    int value = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&buffer_mutex);
    return value;
}