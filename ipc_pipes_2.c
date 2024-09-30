#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe1[2];
    int pipe2[2];
    pid_t c1, c2;
    char parent_msg[] = "Message from parent to child 1";
    char child_msg[] = "Message from child 1 to child2";
    char buffer[1024];

    if (pipe(pipe1) == -1) {
        perror("pipe error!");
        return 1;
    }

    if (pipe(pipe2) == -1) {
        perror("pipe error!");
        return 1;
    }

    c1 = fork();

    if (c1 < 0) {
        perror("fork error!");
        return 1;
    }

    if (c1 == 0) { // child1
        c2 = fork();

        if (c2 < 0) {
            perror("fork error!");
            return 1;
        }

        if (c2 == 0) {
            close(pipe1[0]); // Close pipe1
            close(pipe1[1]);
            close(pipe2[1]); // Close write end of pipe2

            // read from pipe2
            read(pipe2[0], buffer, 1024);
            printf("Child 2 received: %s\n", buffer);

            close(pipe2[0]); // close the read end of pipe2
        } else { // child1 process
          close(pipe1[1]); // close write end of pipe1
          close(pipe2[0]); // close read end of pipe2

          // read from parent
          read(pipe1[0], buffer, 1024);
          printf("Child 1 received: %s\n", buffer);

          write(pipe2[1], child_msg, strlen(child_msg) + 1);

          close(pipe1[0]); //  close read end of pipe1
          close(pipe2[1]); // close write end of pipe2
        }
    } else {
      close(pipe1[0]); // close read end of pipe1
      close(pipe2[0]); // close read end of pipe 2
      close(pipe2[1]); // close write end of pipe 2

      write(pipe1[1], parent_msg, strlen(parent_msg) + 1);

      close(pipe1[1]); // close write end of pipe 1

    }

    return 0;
}