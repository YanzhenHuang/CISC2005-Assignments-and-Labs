#include <stdio.h>
#include <stdlib.h>

struct process
{
    int id;
    int arrival_time;
    int service_time;
    int wait_time;
    int turnaround_time;
};

void calculate_times(struct process *p, int n)
{
    int i, j, time = 0, completed = 0;
    int *remaining_time = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
    {
        remaining_time[i] = p[i].service_time;
    }

    while (completed != n)
    {
        int shortest_index = -1;
        int shortest_time = -1;

        for (j = 0; j < n; j++)
        {
            if (p[j].arrival_time <= time && remaining_time[j] > 0)
            {
                if (shortest_time == -1 || remaining_time[j] < shortest_time)
                {
                    shortest_index = j;
                    shortest_time = remaining_time[j];
                }
            }
        }

        if (shortest_index == -1)
        {
            time++;
            continue;
        }

        remaining_time[shortest_index]--;

        if (remaining_time[shortest_index] == 0)
        {
            completed++;
            int k = shortest_index;
            p[k].wait_time = time - p[k].arrival_time - p[k].service_time + 1;
            if (p[k].wait_time < 0)
            {
                p[k].wait_time = 0;
            }
            p[k].turnaround_time = time - p[k].arrival_time;
        }

        time++;
    }

    free(remaining_time);
}

int main()
{
    int i, n;
    printf("please enter the number of processes:");
    scanf("%d", &n);
    struct process p[n];

    for (i = 0; i < n; i++)
    {
        printf("Please enter the arrival time and execution time of process %d: ", i + 1);
        scanf("%d%d", &p[i].arrival_time, &p[i].service_time);
        p[i].id = i + 1;
        p[i].wait_time = 0;
        p[i].turnaround_time = 0;
    }

    calculate_times(p, n);

    printf("Process\tArrival Time\tService Time\tWait Time\tTurnaround Time\n");
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", p[i].id, p[i].arrival_time, p[i].service_time, p[i].wait_time, p[i].turnaround_time);
        total_wait_time += p[i].wait_time;
        total_turnaround_time += p[i].turnaround_time;
    }
    printf("avg. Wait Time:%.2f\n", (float)total_wait_time / n);
    printf("avg. Turnaround Time:%.2f\n", (float)total_turnaround_time / n);

    return 0;
}
