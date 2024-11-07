#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource_mutex;   // Controls access to the shared resource
pthread_mutex_t read_count_mutex; // Controls access to the read_count variable

int read_count = 0; // Number of active readers
int shared_data = 0; // Shared resource

void* reader(void* arg) {
    int id = *(int*)arg;

    while (1) {
        // Lock to update read_count
        pthread_mutex_lock(&read_count_mutex);
        read_count++;
        if (read_count == 1) {
            // First reader locks the resource
            pthread_mutex_lock(&resource_mutex);
        }
        pthread_mutex_unlock(&read_count_mutex);

        // Reading section
        printf("Reader %d: read shared_data = %d\n", id, shared_data);
        sleep(1); // Simulate reading time

        // Unlock after reading
        pthread_mutex_lock(&read_count_mutex);
        read_count--;
        if (read_count == 0) {
            // Last reader unlocks the resource
            pthread_mutex_unlock(&resource_mutex);
        }
        pthread_mutex_unlock(&read_count_mutex);

        sleep(1); // Simulate time between reads
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        // Request exclusive access to the resource
        pthread_mutex_lock(&resource_mutex);

        // Writing section
        shared_data += id; // Modify shared data (example operation)
        printf("Writer %d: updated shared_data to %d\n", id, shared_data);
        sleep(2); // Simulate writing time

        // Release exclusive access
        pthread_mutex_unlock(&resource_mutex);

        sleep(2); // Simulate time between writes
    }
    return NULL;
}

int main() {
    int num_readers = 5, num_writers = 2;
    pthread_t readers[num_readers], writers[num_writers];
    int ids[num_readers + num_writers];

    pthread_mutex_init(&resource_mutex, NULL);
    pthread_mutex_init(&read_count_mutex, NULL);

    // Create reader threads
    for (int i = 0; i < num_readers; i++) {
        ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < num_writers; i++) {
        ids[num_readers + i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &ids[num_readers + i]);
    }

    // Join threads (not reached in this infinite loop simulation)
    for (int i = 0; i < num_readers; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
