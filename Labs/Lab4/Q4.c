#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int tot_items = 0;

struct kidrec // kid record
{
    int data;
    pthread_t id; // thread id
};

#define NKIDS 10 // static data

void *kidfunc(void *p)
{
    int *ip = (int *)p;
    int tmp, n;
    tmp = tot_items;
    for (n = 500000; n--;)
        tot_items = tmp + *ip;
}

int main()
{
    struct kidrec kids[NKIDS]; // define an array of data-thread pairs

    for (int m = 0; m < NKIDS; ++m) // for every pair
    {
        kids[m].data = m + 1;
        pthread_create(&kids[m].id, NULL, kidfunc, &kids[m].data);
    }
    for (int m = 0; m < NKIDS; ++m)
        pthread_join(kids[m].id, NULL);
    printf("End of Program. Grand Total = %d\n", tot_items);
    return 0;
}
