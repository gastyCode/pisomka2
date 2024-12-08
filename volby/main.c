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

typedef struct consumer_data{
  shared_data* sd_;
  int puttyn_;
  int bash_;
} consumer_data;

int rand_between(int min, int max) {
  return min + rand() % (max + 1 - min);
}

void run_produce(producer_data* pd) {
  for (int i = 0; i < pd->sd_->count_; i++) {
    bool harok;
    syn_buffer_push(pd->sd_->buff_, &harok);
    printf("%d. harok bol vylozeny\n", i+1);
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    int arrival = rand_between(1, 10);
    sleep(arrival);
    printf("%d. volic prisiel do miestnosti\n", i+1);
    
    bool harok;
    syn_buffer_pop(cd->sd_->buff_, &harok);
    harok = rand_between(1, 100) <= 65;
    printf("%d. volic si zobral volebny harok\n", i+1);

    double write = rand_between(100, 150) / 1000.0;
    sleep(write);
    printf("%d. volic zvolil\n", i+1);
    
    if (harok) {
      cd->puttyn_++;
    } else {
      cd->bash_++;
    }
    printf("%d. volic hodil vysledok do urny\n", i+1);
  }
}

void* consume(void* args) {
  run_consume(args);
  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int count = 15;

  if (argc == 2) {
    count = atoi(argv[1]);
  }

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data sd = { &buff, count };
  producer_data pd = { &sd};
  consumer_data cd = { &sd, 0, 0 };

  pthread_t pt, ct;
  pthread_create(&pt, NULL, produce, &pd);
  pthread_create(&ct, NULL, consume, &cd);

  printf("Volby otvorene\n");
  pthread_join(pt, NULL);
  pthread_join(ct, NULL);
  printf("Volby ukoncene\n");

  int all = cd.puttyn_ + cd.bash_;
  double puttyn_result = cd.puttyn_ / (double)all * 100;
  double bash_result = cd.bash_ / (double)all * 100;

  printf("Puttyn, %d, %.2f%%, %s\n", cd.puttyn_, puttyn_result, cd.puttyn_ > cd.bash_ ? "vyhral" : "prehral");
  printf("Ba$h, %d, %.2f%%, %s\n", cd.bash_, bash_result, cd.bash_ > cd.puttyn_ ? "vyhral" : "prehral");

  syn_buffer_destroy(&buff);

  return 0;
}
