#include "syn_buffer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct shared_data {
  syn_buffer* buff_;
  int count_;
} shared_data;

typedef struct producer_data {
  shared_data* sd_;
  int min_;
  int max_;
} producer_data;

typedef struct consumer_data {
  shared_data* sd_;
} consumer_data;

bool is_perfect_square(int x)
{
    int s = sqrt(x);
    return (s * s == x);
}
 
bool is_fibonacci(int n)
{
    return is_perfect_square(5 * n * n + 4)
        || is_perfect_square(5 * n * n - 4);
}

void run_produce(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    int n = pd->min_ + rand() % (pd->max_ + 1 - pd->min_);
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
    if (is_fibonacci(n)) {
      printf("%d\n", n);
    }
  }
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  if (argc != 4) {
    printf("ERROR: Bad arguments count!\n");
    return 1;
  }

  srand(time(NULL));

  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  int count = atoi(argv[3]);

  syn_buffer buff;
  syn_buffer_init(&buff, count);

  shared_data sd = { &buff, count };
  producer_data pd = { &sd, min, max };
  consumer_data cd = { &sd };

  pthread_t pt, ct;
  pthread_create(&pt, NULL, produce, &pd);
  pthread_create(&ct, NULL, consume, &cd);

  pthread_join(pt, NULL);
  pthread_join(ct, NULL);

  syn_buffer_destroy(&buff);

  return 0;
}
