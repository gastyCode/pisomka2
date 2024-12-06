#include "buffer.h"

void buffer_init(buffer* this) {
  this->capacity_ = BUFFER_CAPACITY;
}

void buffer_push(buffer* this, const int input) {
  this->numbers_[this->size_++] = input;
  this->size_ %= this->capacity_;
}

void buffer_pop(buffer* this, int* output) {
  *output = this->numbers_[--this->size_];
  this->size_ %= this->capacity_;
}
