#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
    printf("hello world (pid:%d)\n", (int)getpid());
    int fd = open("myFile.txt", O_CREAT|O_RDWR);
    if(fd == -1 ) {
        printf("Unable to open the file\n exiting....\n");
        return 0;
    }
    
    int rc = fork();
    
    
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        char myChar='a';
        write(fd, &myChar,1);
        printf("writing a character to the file from child\n");
    }
    else {
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int)getpid());
        char myChar='b';
        write(fd, &myChar,1);
        printf("writing a character to the file from parent\n");
    }
return 0;
}
