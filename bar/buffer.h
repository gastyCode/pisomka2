#ifndef BUFFER_H
#define BUFFER_H

#include <stdbool.h>

#define BUFFER_CAP 1

typedef struct buffer {
  bool content_[BUFFER_CAP];
  int capacity_;
  int size_;
  int in_;
  int out_;
} buffer;

void buffer_init(buffer* this);
void buffer_push(buffer* this, const bool* input);
void buffer_pop(buffer* this, bool* output);

#endif
