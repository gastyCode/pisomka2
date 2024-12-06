#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

typedef struct buffer {
  double* times_;
  int capacity_;
  int size_;
  int in_;
  int out_;
} buffer;

void buffer_init(buffer* this, int capacity);
void buffer_destroy(buffer* this);
void buffer_push(buffer* this, double input);
void buffer_pop(buffer* this, double* output);

#endif
