#include "buffer.h"

void buffer_init(buffer* this, int capacity) {
  this->numbers_ = calloc(capacity, sizeof(int));
  this->capacity_ = capacity;
  this->size_ = 0;
  this->in_ = 0;
  this->out_ = 0;
}

void buffer_destroy(buffer* this) {
  free(this->numbers_);
}

void buffer_push(buffer* this, int input) {
  this->numbers_[this->in_++] = input;
  this->size_++;
  this->in_ %= this->capacity_;
}

void buffer_pop(buffer* this, int* output) {
  *output = this->numbers_[this->out_++];
  this->size_--;
  this->out_ %= this->capacity_;
}
