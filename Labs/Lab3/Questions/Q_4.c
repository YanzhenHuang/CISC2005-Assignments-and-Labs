#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid;
    pid = fork();
    if (pid == -1)
        printf("\n ERROR child not created.");
    else if (pid == 0) {
        printf("\n I'm the child!");
        exit(0);
    } else {
        pid_t TCpid;
        TCpid = wait(&status);
        printf("\n I'm the parent!");
        printf("\n The child with pid = %d terminated with a status = %d \n", TCpid, status);
    }
    return 0;
}

