#ifndef BUFFER_H
#define BUFFER_H
#define EVIDENCE_COUNT 3

typedef struct buffer {
  int evidences_[EVIDENCE_COUNT];
  int capacity_;
  int size_;
  int in_;
  int out_;
} buffer;

void buffer_init(buffer* this);
void buffer_push(buffer* this, int input);
void buffer_pop(buffer* this, int* output);

#endif
