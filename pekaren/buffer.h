#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_CAP 2

#include <stdbool.h>

typedef struct buffer {
  bool breads_[BUFFER_CAP];
  int capacity_;
  int size_;
  int in_;
  int out_;
} buffer;

void buffer_init(buffer* this);
void buffer_push(buffer* this, const bool* input);
void buffer_pop(buffer* this, bool* output);

#endif
