#include <stdio.h>
#include <time.h>

#include "syn_buffer.h"

typedef struct shared_data {
  syn_buffer* buff_;
  int count_;
} shared_data;

typedef struct producer_data {
  shared_data* data_;
  int min_;
  int max_;
} producer_data;

typedef struct consumer_data {
  shared_data* data_;
} consumer_data;

void* produce(void* args) {
  run_produce(args);

  return NULL;
}

void run_produce(producer_data* data) {
  for (size_t i = 0; i < data->data_->count_; i++) {
    int n = data->min_ + (rand() % (data->max_ + 1 - data->min_));
    syn_buffer_push(data->data_->buff_, n);
  }
}

void* consume(void* args) {
  run_consume(args);

  return NULL;
}

void run_consume(consumer_data* data) {
  for (size_t i = 0; i < data->data_->count_; i++) {
    int n;
    syn_buffer_pop(data->data_->buff_, &n);

    printf("%d -> ", n);
  
    int c = 2;
    while (n > 1) {
      if (n % c == 0) {
        printf("%d ", c);
        n /= c;
      } else {
        c++;
      }
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  if (argc != 4) {
    printf("Error: Bad arguments!\n");
    return 1;
  }

  srand(time(NULL));

  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  int n = atoi(argv[3]);

  syn_buffer buff;
  syn_buffer_init(&buff);

  shared_data shared_data = { &buff, n };
  producer_data producer_data = { &shared_data, a, b };
  consumer_data consumer_data = { &shared_data };

  pthread_t thread;
  pthread_create(&thread, NULL, produce, &producer_data);

  pthread_join(thread, NULL);
  consume(&consumer_data);

  return 0;
}
