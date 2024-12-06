#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

typedef struct buffer {
  int* numbers_;
  int size_;
  int capacity_;
  int in_;
  int out_;
} buffer;

void buffer_init(buffer* this, int capacity);
void buffer_destroy(buffer* this);
void buffer_push(buffer* this, int input);
void buffer_pop(buffer* this, int* output);

#endif
