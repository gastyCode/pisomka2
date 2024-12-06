#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "syn_buffer.h"

#define THREADS_COUNT 2

typedef struct shared_data {
  syn_buffer* buff_;
} shared_data;

typedef struct producer_data {
  shared_data* sd_;
  double min_;
  double max_;
} producer_data;

typedef struct consumer_data {
  shared_data* sd_;
  double min_;
  double max_;
} consumer_data;

double generate_time(double min, double max) {
  double r = rand() / (double)RAND_MAX;
  return min + r * (max - min);
}

void run_produce(producer_data* pd) {
  while(true) {
    double time = generate_time(pd->min_, pd->max_);
    sleep(time);
    printf("Kuchar: %.2f s\n", time);
    syn_buffer_push(pd->sd_->buff_, time);
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  while(true) {
    double time;
    syn_buffer_pop(cd->sd_->buff_, &time);
    time = generate_time(cd->min_, cd->max_);
    sleep(time);
    printf("Casnik: %.2f s\n", time);
  }
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int capacity = 10;

  if (argc == 2) {
    capacity = atoi(argv[1]);
  }

  syn_buffer buff;
  syn_buffer_init(&buff, capacity);
  shared_data sd = { &buff };
  producer_data pd = { &sd, 0.5, 1.5 };
  consumer_data cd = { &sd, 2, 5 };

  pthread_t threads[THREADS_COUNT];
  for (int i = 0; i < THREADS_COUNT; i++) {
    pthread_create(&threads[i], NULL, consume, &cd);
  }

  for (int i = 0; i < THREADS_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }
  produce(&pd);

  syn_buffer_destroy(&buff);

  return 0;
}
