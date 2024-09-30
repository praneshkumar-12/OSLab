#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    key_t key = ftok("shmfile", 65); // generate key for shared memory

    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); // create shared memory segment, idenitified by an id

    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {// child (reader)
        char *str = (char *)shmat(shmid, (void *)0, 0); // attach to shared memory

        if (str == (char *)-1) { // read and check
            perror("shmat failed");
            return 1;
        }

        printf("Child reads: %s\n", str); // print data

        shmdt(str); // dettach from shared memory
    } else { // parent

        char *str = (char *) shmat(shmid, (void *) 0, 0);
        if (str == (char *) -1) {
            perror("shmat failed");
            return 1;
        }

        char parent_msg[] = "Hello, I am parent!";
        strcpy(str, parent_msg); // write to shared memory
        printf("Parent wrote: %s\n", parent_msg);

        wait(NULL); // wait for child to finish

        shmdt(str); // dettach from shared memory

        shmctl(shmid, IPC_RMID, NULL);
    }
}


