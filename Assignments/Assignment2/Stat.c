#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

struct thread_data
{
    int len;  // length of array
    int *arr; // the array
};

void *average(void *arg)
{
    // Unpack thread data
    struct thread_data *data = (struct thread_data *)arg;
    int len = data->len;
    int *list = data->arr;
    // Calculate Average
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += list[i];
    }
    float avg = sum / len;
    printf("The average value is %f\n", avg);
    pthread_exit(0);
}

void *maximum(void *arg)
{
    // Unpack thread data
    struct thread_data *data = (struct thread_data *)arg;
    int len = data->len;
    int *list = data->arr;
    // Calculate Maximum
    int max = list[0];
    for (int i = 1; i < len; i++)
    {
        if (list[i] > max)
            max = list[i];
    }
    printf("The maximum value is %d\n", max);
    pthread_exit(0);
}

void *minimum(void *arg)
{
    // Unpack thread data
    struct thread_data *data = (struct thread_data *)arg;
    int len = data->len;
    int *list = data->arr;
    // Calculate minimum
    int min = list[0];
    for (int i = 1; i < len; i++)
    {
        if (list[i] < min)
            min = list[i];
    }
    printf("The minimum value is %d\n", min);
    pthread_exit(0);
}

int main()
{
    pthread_t th1, th2, th3; // Thread id
    int num_of_int = 0;

    // Enter the array
    printf("Enter number of integers:");
    scanf("%d", &num_of_int);

    int *nums = (int *)malloc(num_of_int * sizeof(int));

    printf("Please enter the integers: ");
    for (int i = 0; i < num_of_int; i++)
    {
        scanf("%d", &nums[i]);
    }

    // Package thread data
    struct thread_data tdata;
    tdata.len = num_of_int;
    tdata.arr = nums;

    // Create 3 threads
    pthread_create(&th1, NULL, average, &tdata);
    pthread_create(&th2, NULL, maximum, &tdata);
    pthread_create(&th3, NULL, minimum, &tdata);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    return 0;
}