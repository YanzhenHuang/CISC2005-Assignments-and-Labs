/********************Question 1*****************************
 * 1.1 Why the counter value is "Wrong"?                    *
 * 1.2 What happens if the number of iterations is reduced? *
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
#define N_ITERATIONS 1000 // number of iterations

// Global variables
static volatile long counter = 0;

void *thread(void *vargp)
{

    // Enter critical region
    for (long i = 0; i < N_ITERATIONS; i++)
    {
        /* Shared resource */
        counter++;
    }
    // Leave critical region

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if (counter != (2 * N_ITERATIONS))
        printf("Wrong! counter=%ld.\n", counter);
    else
        printf("Correct! counter=%ld.\n", counter);

    return 0;
}
