#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#define BUFFER_CAPACITY 10

typedef struct buffer {
  int numbers_[BUFFER_CAPACITY];
  size_t capacity_;
  size_t size_;
} buffer;

void buffer_init(buffer* this);
void buffer_push(buffer* this, const int input);
void buffer_pop(buffer* this, int* output);

#endif
