#ifndef BUFFER_H
#define BUFFER_H

#include "comb_num.h"

#define BUFFER_CAPACITY 20

typedef struct buffer {
  comb_num data_[BUFFER_CAPACITY];
  int size_;
  int capacity_;
} buffer;

void buffer_init(buffer* this);
void buffer_push(buffer* this, const comb_num* input);
void buffer_pop(buffer* this, comb_num* output);

#endif
