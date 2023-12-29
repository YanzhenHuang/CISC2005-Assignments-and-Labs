#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid1, pid2, pid3;
    pid1 = 0; pid2 = 0; pid3 = 0;
    pid1 = fork();      // creates a child process
    
    if (pid1 == 0) {    // child with pid1 executes this
        pid2 = fork();
        pid3 = fork();
    } else {            // child with pid2 pid 3 executes this
        pid3 = fork();
        if (pid3 == 0)
            pid2 = fork();
    }
    
    if ((pid1 == 0) && (pid2 == 0))     //pid is 3
        printf("level1\n");
    if (pid1 != 0)
        printf("level2\n");
    if (pid2 != 0)
        printf("level3\n");
    if (pid3 != 0)
        printf("level4\n");

    return 0;
}
