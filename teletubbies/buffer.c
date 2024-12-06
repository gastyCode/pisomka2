#include "buffer.h"

void buffer_init(buffer* this) {
  this->capacity_ = BUFFER_CAP;
  this->size_ = 0;
  this->in_ = 0;
  this->out_ = 0;
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
