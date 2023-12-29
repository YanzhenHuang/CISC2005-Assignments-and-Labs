#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <cstring>

#define NUM_PROCESSES 7
#define NUM_PARTITIONS 4

using namespace std;

struct Process
{
    int pid;
    int size;
    Process(int pid, int size)
    {
        this->pid = pid;
        this->size = size;
    }
};
// Fixed partition
void FixedPartition(Process *processes[])
{
    int base_reg[NUM_PARTITIONS] = {0, 15, 35, 65};           // Base Registers
    int partition_size[NUM_PARTITIONS] = {15, 20, 30, 35};    // 4 partition sizes (Sorted)
    bool run[NUM_PROCESSES];                                  // Whether a process is successfully allocated
    int block_occupied_by[NUM_PARTITIONS] = {-1, -1, -1, -1}; // 4 partitions, each number represents pid of process
    int free_space[NUM_PARTITIONS] = {15, 20, 30, 35};        // Free space of each memory block

    // Traverse every process
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        // Process is too large to fit.
        if (processes[i]->size > partition_size[NUM_PARTITIONS - 1])
        {
            cout << "Failed to run process pid = " << processes[i]->pid << endl;
        }

        // For each process, traverse every partition
        for (int j = 0; j < NUM_PARTITIONS; j++)
        {
            if (block_occupied_by[j] == -1 && partition_size[j] >= processes[i]->size) // Find the smallest fitting block that has not been occupied
            {
                block_occupied_by[j] = processes[i]->pid;               // Allocate this memory block to current process
                free_space[j] = partition_size[j] - processes[i]->size; // Calculate free space
                run[i] = 1;                                             // Record that process is allocated

                cout << "Process " << processes[i]->pid << " occupies block " << base_reg[j] << endl;
                break;
            }
        }
        // If all parts are occupied, find the smallest available one.
        if (!run[i])
        {
            for (int j = 0; j < NUM_PARTITIONS; j++) // For each process, traverse every partition
            {
                if (partition_size[j] >= processes[i]->size) // Find the smallest fitting block that has not been occupied
                {
                    // Already occupied by a process, kick it out.
                    cout << "Finish " << block_occupied_by[j] << " with base " << base_reg[j] << endl;
                    block_occupied_by[j] = processes[i]->pid;               // Allocate this memory block
                    free_space[j] = partition_size[j] - processes[i]->size; // Calculate free space
                    run[i] = 1;                                             // Record that process is allocated
                    cout << "Process " << processes[i]->pid << " occupies block " << base_reg[j] << endl;
                    break;
                }
            }
        }
    }
    // Print out free space
    cout << endl
         << "Final Free space:" << endl;
    for (int i = 0; i < NUM_PARTITIONS; i++)
    {
        cout << "Block " << base_reg[i] << "-" << base_reg[i] + partition_size[i] - 1 << " : "
             << "Free Space = " << free_space[i] << " KB" << endl;
    }
}

// Variable sized partition
struct MemBlock
{
    int base;
    int size;
    int id; // Pid of the process occupying this memory block, -1 if its free.
    MemBlock(int base, int size, int id)
    {
        this->base = base;
        this->size = size;
        this->id = id;
    }
};

void VarSizedPartition(Process *processes[])
{
    vector<MemBlock> partitions;            // Base and size of each partition
    vector<bool> run(NUM_PROCESSES, false); // Whether a process is successfully allocated

    // Initialize one large partition that spans the entire memory
    partitions.push_back(MemBlock(0, 100, -1));

    // Traverse every process
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        int sf_block = -1; // Index of the smallest free block that can fit process[i]
        int s_block = -1;  // Index of the smallest block that can fit process[i]

        // Find the smallest free block that can fit process[i]
        for (int j = 0; j < partitions.size(); j++)
        {
            if (partitions[j].id == -1 && partitions[j].size >= processes[i]->size)
            {
                if (sf_block == -1 || partitions[j].size < partitions[sf_block].size)
                {
                    sf_block = j;
                }
            }
        }

        // Find the smallest block that can fit process[i]
        if (sf_block == -1)
        {
            for (int j = 0; j < partitions.size(); j++)
            {
                if (partitions[j].size >= processes[i]->size)
                {
                    if (s_block == -1 || partitions[j].size < partitions[s_block].size)
                    {
                        s_block = j;
                    }
                }
            }
        }

        if (sf_block != -1)
        { // Allocate in the smallest free block that can fit process[i]
            MemBlock old_partition = partitions[sf_block];
            MemBlock process_partition(old_partition.base, processes[i]->size, processes[i]->pid);
            MemBlock free_partition(old_partition.base + processes[i]->size, old_partition.size - processes[i]->size, -1);

            partitions.erase(partitions.begin() + sf_block);
            partitions.insert(partitions.begin() + sf_block, process_partition);

            if (free_partition.size > 0)
            {
                partitions.insert(partitions.begin() + sf_block + 1, free_partition);
            }

            run[i] = true;
            cout << "Process " << i + 1 << " occupies block " << process_partition.base << endl;
        }
        else if (s_block != -1)
        { // Allocate in the smallest block that can fit process[i]
            cout << "Finish process " << partitions[s_block].id << " at base " << partitions[s_block].base << endl;
            MemBlock old_partition = partitions[s_block];
            MemBlock process_partition(old_partition.base, processes[i]->size, processes[i]->pid);
            MemBlock free_partition(old_partition.base + processes[i]->size, old_partition.size - processes[i]->size, -1);

            partitions.erase(partitions.begin() + s_block);
            partitions.insert(partitions.begin() + s_block, process_partition);

            if (free_partition.size > 0)
            {
                partitions.insert(partitions.begin() + s_block + 1, free_partition);
            }

            run[i] = true;
            cout << "Process " << i + 1 << " occupies block " << process_partition.base << endl;
        }
        else
        { // No block can fit process[i]
            cout << "Failed to load process pid = " << i << endl;
        }
    }

    cout << endl
         << "Free Blocks:" << endl;
    for (int i = 0; i < partitions.size(); i++)
    {
        if (partitions[i].id == -1)
        {
            cout << "Block " << partitions[i].base << "-" << partitions[i].base + partitions[i].size - 1 << ": " << partitions[i].size << " KB" << endl;
        }
    }
}

int main()
{
    Process *P1 = new Process(1, 20);
    Process *P2 = new Process(2, 30);
    Process *P3 = new Process(3, 10);
    Process *P4 = new Process(4, 40);
    Process *P5 = new Process(5, 15);
    Process *P6 = new Process(6, 35);
    Process *P7 = new Process(7, 25);

    Process *processes[NUM_PROCESSES] = {P1, P2, P3, P4, P5, P6, P7};

    cout << "\n--- Fixed Partition ---\n";
    FixedPartition(processes);

    cout << "\n--- Variable Partition ---\n";
    VarSizedPartition(processes);
}

// 0-14, 15-34, 35-64, 65-99

/*
pid	length
1	20
2	30
3	10
4	40
5	15
6	35
7	25
*/