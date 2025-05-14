#include "buffer.h"

entity_buffer* create_buffer() {
  entity_buffer* buffer = (entity_buffer*)malloc(sizeof(entity_buffer));
  if (buffer == NULL) {
    perror("Errore allocazione buffer");
    exit(EXIT_FAILURE);
  }
  buffer->head = 0;
  buffer->tail = 0;
  return buffer;
}

void destroy_buffer(entity_buffer* buffer) {
  free(buffer);
}

bool read_from_buffer(entity_buffer* buffer, Entity* dest) {
  pthread_mutex_lock(&mutex);
  if (buffer->head == buffer->tail) {
    pthread_mutex_unlock(&mutex);
    return false; // Buffer vuoto
  }
  *dest = buffer->buffer[buffer->tail];
  buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
  pthread_mutex_unlock(&mutex);
  return true;
}

bool write_from_buffer(entity_buffer* buffer, Entity* dest) {
  pthread_mutex_lock(&mutex);
  if ((buffer->head + 1) % BUFFER_SIZE == buffer->tail) {
    pthread_mutex_unlock(&mutex);
    return false; // Buffer pieno
  }
  buffer->buffer[buffer->head] = *dest;
  buffer->head = (buffer->head + 1) % BUFFER_SIZE;
  pthread_mutex_unlock(&mutex);
  return true;
}
