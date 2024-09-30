#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

    // int pipe(int fds[2]);
    //
    // Parameters :
    // fd[0] will be the fd(file descriptor) for the
    // read end of pipe.
    // fd[1] will be the fd for the write end of pipe.
    // Returns : 0 on Success.
    // -1 on error.

    int pipefds[2];
    pid_t pid;
    char write_msg[] = "Hello from Parent!";
    char read_msg[100];

    // Create a pipe
    if (pipe(pipefds) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {  // Parent process
        close(pipefds[0]);  // Close the read end of the pipe
        write(pipefds[1], write_msg, strlen(write_msg) + 1);  // Write to pipe
        close(pipefds[1]);  // Close the write end of the pipe
    }
    else {  // Child process
        close(pipefds[1]);  // Close the write end of the pipe
        read(pipefds[0], read_msg, sizeof(read_msg));  // Read from pipe
        printf("Child received: %s\n", read_msg);  // Print the message from the parent
        close(pipefds[0]);  // Close the read end of the pipe
    }

    return 0;
}
