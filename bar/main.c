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
} producer_data;

typedef struct consumer_data {
  shared_data* sd_;
  int time_min_;
  int time_max_;
} consumer_data;

void run_produce(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    int drink = rand() % 2 + 1;
    sleep(drink);
    bool d = true;
    syn_buffer_push(pd->sd_->buff_, &d);
    if (drink == 1) {
      printf("Barman polozil druhy drink\n");
    } else {
      printf("Barman polozil prvy drink\n");
    }
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    int time = cd->time_min_ + rand() % (cd->time_max_ + 1 - cd->time_min_);
    sleep(time);
    bool d;
    syn_buffer_pop(cd->sd_->buff_, &d);
    printf("%d. zakaznik: Berie drink z baru\n", i+1);
  } 
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int customers = 10;

  if (argc == 2) {
    customers = atoi(argv[1]);
  }

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data sd = { &buff, customers };
  producer_data pd = { &sd };
  consumer_data cd = { &sd, 1, 8 };

  pthread_t pt, ct;
  pthread_create(&pt, NULL, produce, &pd);
  pthread_create(&ct, NULL, consume, &cd);

  printf("Bar otvoreny\n");

  pthread_join(pt, NULL);
  pthread_join(ct, NULL);

  printf("Bar zatovreny\n");

  syn_buffer_destroy(&buff);

  return 0;
}
