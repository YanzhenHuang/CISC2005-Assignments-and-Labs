#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int myVariable = 100;
    printf("hello world (pid:%d)\n", (int)getpid());
    printf("Value of myVariable before fork is %d\n", myVariable);
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit`
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        myVariable = 200;
        printf("The changed value of myVariable from child is %d\n", myVariable);
    } else {
        myVariable = 300;
        printf("The changed value of myVariable from parent is %d\n", myVariable);
    }
return 0;
}
