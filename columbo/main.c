#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "syn_buffer.h"

typedef struct shared_data {
  syn_buffer* buff_;
  int count_;
} shared_data;

typedef struct producer_data {
  shared_data* sd_;
} producer_data;

typedef struct consumer_data {
  shared_data* sd_;
  int first_;
  int second_;
  int invalid_;
} consumer_data;

void run_producer(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    int n = rand() % 3;
    syn_buffer_push(pd->sd_->buff_, n);
  }
}

void* producer(void* args) {
  run_producer(args);
  return NULL;
}

void run_consumer(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    int n;
    syn_buffer_pop(cd->sd_->buff_, &n);
    if (n == 1) {
      cd->first_++;
    } else if (n == 2) {
      cd->second_++;
    } else {
      cd->invalid_++;
    }
  }
}

void* consumer(void* args) {
  run_consumer(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data sd = { &buff, 50 };
  producer_data pd = { &sd };
  consumer_data cd = { &sd, 0, 0, 0 };

  pthread_t thread;
  pthread_create(&thread, NULL, consumer, &cd);

  producer(&pd);
  pthread_join(thread, NULL);

  printf("First: %d, Second: %d, Invalid: %d\n", cd.first_, cd.second_, cd.invalid_);

  if (cd.first_ > cd.second_) {
    printf("First one is guilty\n");
  } else {
    printf("Second one is guilty\n");
  }

  syn_buffer_destroy(&buff);

  return 0;
}
