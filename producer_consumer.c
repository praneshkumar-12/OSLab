#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

# define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;

pthread_mutex_t mutex;

void insert_item(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
}

int remove_item() {
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    return item;
}

void* producer(void* params) {
    int item;

    while (1) {
        sleep(1);
        item = rand() % 100;

        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        insert_item(item);

        printf("Produced item = %d\n", item);

        pthread_mutex_unlock(&mutex);

        sem_post(&full);
    }
}

void* consumer(void* params) {
    int item;

    while (1) {
        sleep(2);

        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int consumed_item = remove_item();

        printf("Consumed item = %d\n", consumed_item);

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);
    }
}

int main() {
    pthread_t producer_thread, consumer_thread;

    sem_init(&empty,
        0, //is_shared: if 0 shared b/w threads
        BUFFER_SIZE // initially BUFFER_SIZE slots are empty
        );
    sem_init(&full, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}