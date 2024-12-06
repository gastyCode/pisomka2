#ifndef SYN_BUFFER_H
#define SYN_BUFFER_H

#include "buffer.h"
#include <pthread.h>

typedef struct syn_buffer {
  buffer buff_;
  pthread_mutex_t mut_;
  pthread_cond_t consume_;
  pthread_cond_t produce_;
} syn_buffer;

void syn_buffer_init(syn_buffer* this);
void syn_buffer_destroy(syn_buffer* this);
void syn_buffer_push(syn_buffer* this, const int input);
void syn_buffer_pop(syn_buffer* this, int* output);

#endif