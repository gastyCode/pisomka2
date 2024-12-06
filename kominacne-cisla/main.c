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
  int a_;
  int b_;
} producer_data;

typedef struct consumer_data {
  shared_data* sd_;
} consumer_data;

int generate_random_between(int min, int max) {
  return min + rand() % (max + 1 - min);
}

int factorial(int n) {
  if (n < 0) return 0;
  else if (n == 0 || n == 1) return 1;
  return n * factorial(n-1);
}

void run_produce(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    int s = generate_random_between(pd->a_, pd->b_);
    int r = generate_random_between(pd->a_, pd->b_);
    comb_num cn;
    if (s >= r) {
      cn.s_ = s;
      cn.r_ = r;
    } else {
      cn.s_ = r;
      cn.r_ = s;
    }
    syn_buffer_push(pd->sd_->buff_, &cn);
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    comb_num cn;
    syn_buffer_pop(cd->sd_->buff_, &cn);
    int sr = cn.s_ - cn.r_;
    int up = factorial(cn.s_);
    int down = factorial(cn.r_) * factorial(sr);
    int result = up / down;
    printf("(%dC%d) => %d\n", cn.s_, cn.r_, result);
  } 
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  if (argc != 4) {
    printf("Error: Bad arguments count!\n");
    return 1;
  }

  srand(time(NULL));

  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  int n = atoi(argv[3]);

  syn_buffer buffer;
  syn_buffer_init(&buffer);

  shared_data sd = { &buffer, n };
  producer_data pd = { &sd, a, b };
  consumer_data cd = { &sd };

  pthread_t producer_thread;
  pthread_create(&producer_thread, NULL, produce, &pd);

  pthread_join(producer_thread, NULL);
  consume(&cd);

  syn_buffer_destroy(&buffer);

  return 0;
}
