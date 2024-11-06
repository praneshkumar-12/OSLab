#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS];   // One semaphore per fork
pthread_t philosophers[NUM_PHILOSOPHERS];
int philosopher_ids[NUM_PHILOSOPHERS];

void* philosopher(void* num) {
    int id = *(int*)num;
    
    // Infinite loop to simulate thinking and eating
    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 2 + 1);  // Simulate thinking

        // Take forks (semaphores)
        printf("Philosopher %d is hungry and trying to pick up forks.\n", id);
        
        sem_wait(&forks[id]);  // Pick up left fork
        sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);  // Pick up right fork

        // Eating section
        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 2 + 1);  // Simulate eating

        // Put down forks
        sem_post(&forks[id]);  // Put down left fork
        sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]);  // Put down right fork

        printf("Philosopher %d has finished eating and is putting down forks.\n", id);
    }
}

int main() {
    // Initialize semaphores for each fork
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
        philosopher_ids[i] = i;
    }

    // Create threads for each philosopher
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Wait for each philosopher thread to complete (which won't happen here)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores (never reached in this infinite simulation)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
