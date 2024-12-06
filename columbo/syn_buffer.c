#include "syn_buffer.h"

void syn_buffer_init(syn_buffer* this) {
  buffer_init(&this->buff_);
  pthread_mutex_init(&this->mut_, NULL);
  pthread_cond_init(&this->consume_, NULL);
  pthread_cond_init(&this->produce_, NULL);
}

void syn_buffer_destroy(syn_buffer* this) {
  pthread_mutex_destroy(&this->mut_);
  pthread_cond_destroy(&this->consume_);
  pthread_cond_destroy(&this->produce_);
}

void syn_buffer_push(syn_buffer* this, int input) {
  pthread_mutex_lock(&this->mut_);
  while(this->buff_.size_ == this->buff_.capacity_) {
    pthread_cond_wait(&this->produce_, &this->mut_);
  }
  buffer_push(&this->buff_, input);
  pthread_cond_signal(&this->consume_);
  pthread_mutex_unlock(&this->mut_); 
}

void syn_buffer_pop(syn_buffer* this, int* output) {
  pthread_mutex_lock(&this->mut_);
  while(this->buff_.size_ == 0) {
    pthread_cond_wait(&this->consume_, &this->mut_);
  }
  buffer_pop(&this->buff_, output);
  pthread_cond_signal(&this->produce_);
  pthread_mutex_unlock(&this->mut_);
}
