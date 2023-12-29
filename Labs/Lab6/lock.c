/**********************Question 4*********************************
 * 4.1 What's the difference between the 2 lock algorithms in the *
 *     "acquire" function?                                        *
 * 4.2 Does the 1st algorithm work? Why?                          *
 * 4.3 Does the 2nd algorithm work? How does the "test and set"   *
 *     operation work?                                            *
 ******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define NUM_THREADS 5
#define N_ITERATIONS 100000000

// Global variables
static volatile long counter = 0;

// Semantic:
// test_and_set(int *addr, int newval)
// {
//     int old = *addr;
//     *addr = newval;
//     return old;
// }

// Implementation in x86
int test_and_set_x86(volatile int *addr, int newval)
{
    int result = newval;
    asm volatile("lock; xchg %0, %1"
                 : "+m"(*addr), "=r"(result)
                 : "1"(newval)
                 : "cc");
    return result;
}

void acquire(int *lock)
{
    /***********1st algorithm*******/
    // while (*lock)
    // {
    //     // Wait
    // }
    // *lock = TRUE;
    /*******************************/

    /*********2nd algorithm*********/
    while (test_and_set_x86(lock, TRUE) == TRUE)
    {
        // Wait
    }

    while (__sync_lock_test_and_set(lock, TRUE) == TRUE)
    {
        // Wait
    }
    /*******************************/
}

void release(int *lock)
{
    *lock = FALSE;
}

// Increment function
void *increment(void *arg)
{

    int *lock = (int *)arg;
    // Try to enter critical region
    acquire(lock);

    for (long i = 0; i < N_ITERATIONS; i++)
    {
        counter++;
    }

    // Leave critical section
    release(lock);

    return NULL;
}

int main()
{
    int lock = FALSE; // Share variable

    pthread_t threads[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment, &lock);
    }
    printf("Create %d threads.\n", NUM_THREADS);

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("counter=%ld.\n", counter);

    return 0;
}
