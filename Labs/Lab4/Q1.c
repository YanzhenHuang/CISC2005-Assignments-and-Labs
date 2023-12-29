#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void kidfunc()
{
    printf("Kid PID is %d, thread ID is %d\n", getpid(), pthread_self());
}

int main()
{
    pthread_t kid;
    pthread_create(&kid, NULL, kidfunc, NULL);
    // addr, --, what to execute,
    printf("Parent PID is %d, thread ID is %d\n", getpid(), pthread_self());
    pthread_join(kid, NULL);
    printf("Complete!\n");
    return 0;
}
