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
  int odd_;
} consumer_data;

void run_produce(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    int n = rand();
    syn_buffer_push(pd->sd_->buff_, n);
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    int n;
    syn_buffer_pop(cd->sd_->buff_, &n);
    if (n % 2 != 0) {
      cd->odd_++; 
    }
  }
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data sd = { &buff, 1000 };
  producer_data pd = { &sd };
  consumer_data cd = { &sd, 0 };

  pthread_t pt, ct;
  pthread_create(&pt, NULL, produce, &pd);
  pthread_create(&ct, NULL, consume, &cd);

  pthread_join(pt, NULL);
  pthread_join(ct, NULL);

  int part = (int)(sd.count_ * 0.6);
  printf("odd numbers count: %d\n", cd.odd_);
  if (cd.odd_ > part) {
    printf("Tinky-Winky win\n");
  } else {
    printf("Laa-Laa win\n");
  }

  syn_buffer_destroy(&buff);

  return 0;
}
