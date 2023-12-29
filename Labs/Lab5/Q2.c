#include <stdio.h>

struct process
{
    int id;
    int arrival_time;
    int service_time;
    int wait_time;
    int turnaround_time;
    int service_save;
};

void calculate_times(struct process *p, int n)
{
    int i, j;
    int current_time = 0;

    for (i = 0; i < n; i++)
    {
        // Find the process with the shortest execution time among all processes whose arrival time is less than or equal to the current time
        int shortest_index = -1;
        int shortest_time = -1;
        for (j = 0; j < n; j++)
        {
            if (p[j].arrival_time <= current_time && p[j].service_time > 0) //** blank
            {
                if (shortest_time == -1 || p[j].service_time < shortest_time)
                {
                    shortest_index = j;
                    shortest_time = p[j].service_time;
                }
            }
        }

        if (shortest_index == -1)
        {
            current_time++;
            i--;
            continue;
        }

        // Update the wait time and current time
        p[shortest_index].wait_time = current_time - p[shortest_index].arrival_time;
        current_time += p[shortest_index].service_time; // **blank
        p[shortest_index].turnaround_time = current_time - p[shortest_index].arrival_time;
        p[shortest_index].service_time = 0; //
    }
}

int main()
{
    int i, n;
    printf("please enter the number of processes:");
    scanf("%d", &n);
    struct process p[n];

    // Input process information
    for (i = 0; i < n; i++)
    {
        printf("Please enter the arrival time and service time of process %d: ", i + 1);
        scanf("%d%d", &p[i].arrival_time, &p[i].service_time);
        p[i].service_save = p[i].service_time;
        p[i].id = i + 1;
        p[i].wait_time = 0;
        p[i].turnaround_time = 0;
    }

    calculate_times(p, n);

    // calculate average wait time.

    double avg_wait_time = 0;
    for (i = 0; i < n; i++)
    {
        avg_wait_time += p[i].wait_time;
    }
    avg_wait_time /= n;

    // Ouput results
    printf("Process\tArrival Time\tService Time\tWait Time\tTurnaround Time\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", p[i].id, p[i].arrival_time, p[i].service_save, p[i].wait_time, p[i].turnaround_time);
    }
    printf("avg. Wait Time:%f\n", avg_wait_time);
    return 0;
}
