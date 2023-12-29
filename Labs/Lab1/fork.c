#include <stdio.h>
int main(int argc, char *argv[])
{
    char *name = argv[0];
    int child_pid = fork();
    if (child_pid == 0)
    {
        printf("Child of % s is % d\n", name, getpid());
        return 0;
    }
    else
    {
        printf("My child is % d\n", child_pid);
        return 0;
    }
}