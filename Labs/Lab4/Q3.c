#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int this_is_global;

void thread_func(void *dummy)
{
    int local_thread;
    printf("Thread %d, pid %d, addresses: &this_is_global: %X, &local: %X\n",
           pthread_self(), getpid(), &this_is_global, &local_thread);

    this_is_global++;

    printf("In Thread %d, incremented this_is_global=%d\n", pthread_self(),
           this_is_global);

    pthread_exit(0);
}

int main()
{
    // thread test
    pthread_t thread1, thread2;
    printf("First, we create two threads to see better what context they share...\n");

    this_is_global = 1000;
    printf("Set this_is_global=%d\n", this_is_global);

    // Create two threads
    pthread_create(&thread1, NULL, (void *)&thread_func, (void *)NULL);
    pthread_create(&thread2, NULL, (void *)&thread_func, (void *)NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("After threads, this_is_global=%d\n", this_is_global);
    printf("\n");
    printf("After testing threads, let's call fork..\n");

    // process test
    int local_main = 17;
    this_is_global = 17;
    printf("Before fork(), local_main=%d, this_is_global=%d\n", local_main, this_is_global);
    fflush(stdout);
    int pid = fork();
    if (pid == 0)
    {
        printf("In child, pid %d: &this_is_global: %X, &local_main: %X\n", getpid(), &this_is_global, &local_main);
        local_main = 13;
        this_is_global = 23;
        printf("In child, local main=%d, this_is_global=%d\n", local_main, this_is_global);
        exit(0);
    }
    else
    {
        printf("In parent, pid %d: &this_is_global: %X, &local_main: %X\n", getpid(), &this_is_global, &local_main);
        wait(NULL);
        printf("In parent, local_main=%d, this_is_global=%d\n", local_main, this_is_global);
        exit(0);
    }
}
