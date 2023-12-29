#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int glob_data = 5;

void *kidfunc()
{
    glob_data = 15;
}
int main()
{
    pthread_t kid;
    printf("Start of program. Global data = %d.\n", glob_data);
    pthread_create(&kid, NULL, kidfunc, NULL); // create a thread.
    glob_data = 10;
    pthread_join(kid, NULL);
    printf("End of program. Global data = %d.\n", glob_data);
    return 0;
}
