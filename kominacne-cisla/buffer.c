#include "buffer.h"

void buffer_init(buffer* this) {
  this->capacity_ = BUFFER_CAPACITY;
  this->size_ = 0;
}

void buffer_push(buffer* this, const comb_num* input) {
  this->data_[this->size_++] = *input;
  this->size_ %= this->capacity_;
}

void buffer_pop(buffer* this, comb_num* output) {
  *output = this->data_[--this->size_];
  this->size_ %= this->capacity_;
}

