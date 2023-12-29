#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct process
{
    int pid;     // Process ID
    int arrive;  // Arrival Time
    int service; // Service Time
};

// Sort in ascending order of arrival time
int cmp(const void *a, const void *b)
{
    struct process *p1 = (struct process *)a;
    struct process *p2 = (struct process *)b;
    return p1->arrive - p2->arrive;
}

int main()
{
    int n, i;
    // Number of processes
    printf("please enter the number of processes:");
    scanf("%d", &n);
    struct process *proc = malloc(sizeof(struct process) * n);
    int *wait_time = malloc(sizeof(int) * n);
    int *turnaround_time = malloc(sizeof(int) * n);
    int current_time = 0;

    // Info of each process
    printf("please enter the info for each process, with three numbers on each line representing the process ID, arrival time, and service time(separated by Space).：\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d %d %d", &proc[i].pid, &proc[i].arrive, &proc[i].service);
    }

    // Sort in ascending order of arrival time
    qsort(proc, n, sizeof(struct process), cmp);

    for (i = 0; i < n; i++)
    {
        if (current_time < proc[i].arrive)
        {
            current_time = proc[i].arrive;
        }
        wait_time[i] = current_time - proc[i].arrive;
        current_time += proc[i].service;
        turnaround_time[i] = wait_time[i] + proc[i].service;
    }

    printf("The wait time and turnaround time for each process are as follows:\n");
    double avg_wait_time = 0;
    for (i = 0; i < n; i++)
    {
        avg_wait_time += wait_time[i];
        printf("%d %d %d\n", proc[i].pid, wait_time[i], turnaround_time[i]);
    }
    printf("avg. Wait Time:%f\n", avg_wait_time);
    avg_wait_time /= n;
    free(proc);
    free(wait_time);
    free(turnaround_time);
    return 0;
}
