#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TRUE 1
typedef int buffer_item;
#define BUFFER_SIZE 5
#define SLEEP_SECONDS 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

// Global variables
buffer_item START_NUMBER = 0;
buffer_item buffer[BUFFER_SIZE];
sem_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

int insert_item(buffer_item item)
{
	buffer[insertPointer] = item;
	insertPointer = (insertPointer + 1) % BUFFER_SIZE;
	return 0;
}

int remove_item(buffer_item *item)
{
	*item = buffer[removePointer];
	removePointer = (removePointer + 1) % BUFFER_SIZE;
	return 0;
}

void initialization(){
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, BUFFER_SIZE);
	sem_init(&empty, 0, 0);
}

void *producer(void *arg)
{
	int thread_id = *(int*)arg;
	buffer_item item;

	while(TRUE) {
		sleep(1);
		sem_wait(&full);
		sem_wait(&mutex);

		item = START_NUMBER++;
		insert_item(item);

		printf("Producer %d produced %d \n", thread_id, item);

		sem_post(&mutex);
		sem_post(&empty);
	}

	return NULL;
}

void *consumer(void *arg)
{
	int thread_id = *(int*)arg;
	buffer_item item;

	while(TRUE){
		sleep(1);
		sem_wait(&empty);
		sem_wait(&mutex);

		remove_item(&item);
		printf("Consumer %d consumed %d \n", thread_id, item);

		sem_post(&mutex);
		sem_post(&full);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	initialization();
	pthread_t pid, cid;
	int producer_id[NUM_PRODUCERS];
	for(int i = 0; i < NUM_PRODUCERS; i++){
		producer_id[i] = i + 1;
		pthread_create(&pid, NULL, &producer, &producer_id[i]);
	}

	int consumer_id[NUM_CONSUMERS];
	for(int j = 0; j < NUM_CONSUMERS; j++){
		consumer_id[j] = j + 1;
		pthread_create(&cid, NULL, &consumer, &consumer_id[j]);
	}

	/* Sleep for the specified amount of time in seconds */
	sleep(SLEEP_SECONDS);

	/* Exit the program */
	printf("Exit the program\n");

	return 0;
}
