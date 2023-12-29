#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {  // fork failure
        fprintf(stderr, "Fork Failed");
        exit(-1);
    } else if (pid == 0) {  //child process
        char ** args;
        args = malloc(3 * sizeof(char*));
        args[0] = "ls";
        args[1] = "-l";
        execv("/bin/ls", args);
    } else {    //parent process
        wait(NULL);
        printf("Child Complete\n");
        exit(0);
    }
}
