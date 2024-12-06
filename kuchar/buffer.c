#include "buffer.h"

void buffer_init(buffer* this, int capacity) {
  this->times_ = calloc(capacity, sizeof(double));
  this->capacity_ = capacity;
  this->size_ = 0;
  this->in_ = 0;
  this->out_ = 0;
}

void buffer_destroy(buffer* this) {
  free(this);
}

void buffer_push(buffer* this, double input) {
  this->times_[this->in_++] = input;
  this->size_++;
  this->in_ %= this->capacity_;
}

void buffer_pop(buffer* this, double* output) {
  *output = this->times_[this->out_++];
  this->size_--;
  this->out_ %= this->capacity_;
}
