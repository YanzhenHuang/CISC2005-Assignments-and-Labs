#include <stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>
#include <unistd.h>

/* struct contains shared semaphores */
typedef struct {
  sem_t A;
  sem_t B;
} SEMAPHORE;

void *first (void * args)
{
  SEMAPHORE *sem = (SEMAPHORE *) args;
  /* Acquire "A" before "B" */
  printf("Job started in the first thread.\n");
  sem_wait(&sem->A);
  sleep(1);
  while (-1 == sem_trywait(&sem->B))
  {
    printf("The first thread has acquired 'A' and waits for 'B'.\n");
    sleep(1);
  }
  /* other code here */
  return NULL;
}

void *second (void * args)
{
  SEMAPHORE *sem = (SEMAPHORE *) args;
  /* Acquire "B" before "A" */
  printf("Job started in the second thread.\n");
  sem_wait(&sem->B);
  sleep(1);
  while (-1 == sem_trywait(&sem->A))
  {
    printf("The second thread has acquired 'B' and waits for 'A'.\n");
    sleep(1);
  }
  /* other code here */
  return NULL;
}

int main()
{
    SEMAPHORE sem;
    // Initialize the semaphore
    sem_init(&sem.A, 0, 1);
    sem_init(&sem.B, 0, 1);
    
    printf("sem_init.\n");
    // Create threads
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, first, &sem);
    pthread_create(&tid2, NULL, second, &sem);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy the semaphore
    sem_destroy(&sem.A);
    sem_destroy(&sem.B);

    return 0;
}