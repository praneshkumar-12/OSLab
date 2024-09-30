#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

int main() {
    key_t key = ftok("shmfile", 65); // Generate unique key

    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); // Create shared memory segment

    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *str = (char *) shmat(shmid, (void *)0, 0); // Attach to shared memory
    if (str == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Write data to shared memory
    char server_msg[] = "Hello from Server!";
    strcpy(str, server_msg);
    printf("Server wrote: %s\n", server_msg);

    shmdt(str); // Detach from shared memory

    return 0;
}
