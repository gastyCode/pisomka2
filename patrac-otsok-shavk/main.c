#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
  int evidences_;
} consumer_data;

int rnd_between(int min, int max) {
  return min + rand() % (max + 1 - min);
}

void run_produce(producer_data* pd) {
  sleep(pd->time_);
  printf("Podozrivy vytvoril %d dokazov\n", pd->sd_->count_);
  for (int i = 0; i < pd->sd_->count_; i++) {
    bool valid = rnd_between(1, 10) < 2;
    syn_buffer_push(pd->sd_->buff_, &valid);
    printf("Podozrivy polozil %d. dokaz\n", i+1);
  }
}

void* produce(void* args) {
  run_produce(args);
  return NULL;
}

void run_consume(consumer_data* cd) {
  for (int i = 0; i < cd->sd_->count_; i++) {
    printf("%d. detektiv zacal patranie\n", i+1);
    int arrival = rnd_between(5, 10);
    int leave = rnd_between(4, 10);
    bool arch = rnd_between(1, 100) <= 40;
    bool steal = rnd_between(0, 1) < 1 && !arch;
    sleep(arrival);
    if (arch) {
      printf("%d. detektiv sa dostavil na miesto a po ceste nasiel Arch\n", i+1);
    } else {
      printf("%d. detektiv sa dostavil na miesto\n", i+1);
    }
    bool evidence;
    syn_buffer_pop(cd->sd_->buff_, &evidence);
    if (evidence) {
      printf("%d. detektiv nasiel platny dokaz\n", i+1);
    } else {
      printf("%d. detektiv nasiel neplatny dokaz\n", i+1);
    }
    sleep(leave);
    if (evidence && steal) {
      evidence = false;
      printf("%d. detektivovi vymenili dokaz za neplatny\n", i+1);
    }
    printf("%d. detektiv ukoncil patranie\n", i+1);
    if (evidence) {
      cd->evidences_++;
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

  int detectives = 10, time = 6;

  if (argc == 3) {
    detectives = atoi(argv[1]);
    time = atoi(argv[2]);
  }
  if (argc == 2) {
    detectives = atoi(argv[1]);
  }

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data sd = { &buff, detectives };
  producer_data pd = { &sd, time };
  consumer_data cd = { &sd, 0 };

  pthread_t pt, ct;
  pthread_create(&pt, NULL, produce, &pd);
  pthread_create(&ct, NULL, consume, &cd);
  
  printf("Najdite Kefasa, patranie sa zacina!\n");
  pthread_join(pt, NULL);
  pthread_join(ct, NULL);
  printf("Dokazy boli pozbierane!\n");

  if (cd.evidences_ > 0) {
    printf("Pachatel bol usvedceny\n");
  } else {
    printf("Pachatel nebol usvedceny\n");
  }

  syn_buffer_destroy(&buff);

  return 0;
}
