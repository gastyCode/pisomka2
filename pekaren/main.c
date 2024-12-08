#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "syn_buffer.h"

typedef struct shared_data {
  syn_buffer* buff_;
  int count_;
} shared_data;

typedef struct producer_data {
  shared_data* sd_;
  int time_;
} producer_data;

typedef struct consumer_data {
  shared_data* sd_;
  int min_time_;
  int max_time_;
} consumer_data;

void run_produce(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    sleep(pd->time_);
    bool bread = true;
    syn_buffer_push(pd->sd_->buff_, &bread);
    printf("Chlieb napeceny\n");
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    int time = cd->min_time_ + rand() % (cd->max_time_ + 1 - cd->min_time_);
    sleep(time);
    bool bread;
    syn_buffer_pop(cd->sd_->buff_, &bread);
    printf("%d. zakaznik: Chlieb kupeny\n", i+1);
  }
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  int customers, time;

  if (argc == 3) {
    customers = atoi(argv[1]);
    time = atoi(argv[2]);
  } else if (argc == 2) {
    customers = atoi(argv[1]);
    time = 4;
  } else {
    customers = 10;
    time = 4;
  }

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data sd = { &buff, customers };
  producer_data pd = { &sd, time };
  consumer_data cd = { &sd, 2, 6 };

  pthread_t pt, ct;
  pthread_create(&pt, NULL, produce, &pd);
  pthread_create(&ct, NULL, consume, &cd);

  printf("Pekaren otvorena\n");

  pthread_join(pt, NULL);
  pthread_join(ct, NULL);

  printf("Pekaren zatvorena\n");

  syn_buffer_destroy(&buff);

  return 0;
}
