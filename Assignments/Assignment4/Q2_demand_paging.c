#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define NUM_REQUESTS 12

int requests[NUM_REQUESTS] = {10, 11, 104, 170, 73, 309, 185, 245, 246, 434, 458, 364};

// First-in-first-out
void FIFO(int mem_sz, int page_sz)
{
    int num_frames = mem_sz / page_sz;
    int *frames = malloc(num_frames * sizeof(int));
    int curr = 0; // Current replacement candidate
    int hits = 0; // Hit time

    // Initialize an empty page frame
    memset(frames, -1, num_frames * sizeof(int));

    // Traverse every requests
    for (int i = 0; i < NUM_REQUESTS; i++)
    {
        int page_num = requests[i] / page_sz;
        bool hit = false;

        // Find whether the page number is in TLB
        for (int j = 0; j < num_frames; j++)
        {
            if (frames[j] == page_num)
            {
                hit = true;
                hits++;
                break;
            }
        }

        // TLB miss
        if (!hit)
        {
            frames[curr] = page_num;
            curr = (curr + 1) % num_frames;
        }
    }
    // Stat
    float hit_rate = (float)hits / NUM_REQUESTS;
    printf("FIFO hit rate: %f", hit_rate);
}

// Least-recently-used
void LRU(int mem_sz, int page_sz)
{
    int num_frames = mem_sz / page_sz;
    int *frames = malloc(num_frames * sizeof(int));          // Which physical page frame is occupied by which page
    int *recent_use_time = malloc(num_frames * sizeof(int)); // Time since last used
    int hits = 0;                                            // Hit time
    int lru = 0;                                             // Index of the least recently used

    // Initialize an empty page frame
    memset(frames, -1, num_frames * sizeof(int));

    // Traverse every requests
    for (int i = 0; i < NUM_REQUESTS; i++)
    {
        int page_num = requests[i] / page_sz; // page number
        bool hit = false;

        // Increase time by 1 for every occupied page frame
        for (int k = 0; k < num_frames; k++)
        {
            if (frames[k] != -1)
                recent_use_time[k]++;
        }

        // Find whether the page number is in TLB
        for (int j = 0; j < num_frames; j++)
        {
            if (frames[j] == page_num)
            {
                hit = true;
                hits++;
                recent_use_time[j] = 0; // Restore recent use time
                break;
            }
        }

        // TLB miss, use LRU algorithm
        if (!hit)
        {
            // Find the least recently used page
            for (int p = 0; p < num_frames - 1; p++)
            {
                if (recent_use_time[lru] > recent_use_time[p + 1])
                    lru = p + 1;
            }
            recent_use_time[lru] = 0;
            frames[lru] = page_num; // Let current page number occupy the page frame (if already occupied, automatically overwrite)
        }
    }
    // Stat
    float hit_rate = (float)hits / NUM_REQUESTS;
    printf("LRU hit rate: %f", hit_rate);
}

int main()
{
    printf("\n--- Case (a): Memory size 200, each with size 100 ---\n");
    FIFO(200, 100);
    printf("\n");
    LRU(200, 100);

    printf("\n");

    printf("\n--- Case (b): Memory size 200, each with size 20 ---\n");
    FIFO(200, 20);
    printf("\n");
    LRU(200, 20);

    return 0;
}
