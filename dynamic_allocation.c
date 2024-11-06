#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_BLOCKS 5   // Number of available memory blocks
#define NUM_PROCESSES 4 // Number of processes to allocate memory

// Memory block sizes and processes
int blockSize[NUM_BLOCKS] = {100, 500, 200, 300, 600};
int processSize[NUM_PROCESSES] = {212, 417, 112, 426};

// Array to store the block assigned to each process
int allocation[NUM_PROCESSES];

// Function to initialize allocations to -1 (indicating unallocated)
void initializeAllocation() {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        allocation[i] = -1;
    }
}

// First Fit allocation
void firstFit() {
    initializeAllocation();
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_BLOCKS; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }
}

// Best Fit allocation
void bestFit() {
    initializeAllocation();
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int bestIdx = -1;
        for (int j = 0; j < NUM_BLOCKS; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }
}

// Worst Fit allocation
void worstFit() {
    initializeAllocation();
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int worstIdx = -1;
        for (int j = 0; j < NUM_BLOCKS; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }
}

// Function to print allocation result
void printAllocation() {
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (allocation[i] != -1) {
            printf("%d\t\t%d\t\t%d\n", i + 1, processSize[i], allocation[i] + 1);
        } else {
            printf("%d\t\t%d\t\tNot Allocated\n", i + 1, processSize[i]);
        }
    }
}

// Function to reset block sizes (for re-running different strategies)
void resetBlockSizes() {
    int originalBlockSize[NUM_BLOCKS] = {100, 500, 200, 300, 600};
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blockSize[i] = originalBlockSize[i];
    }
}

int main() {
    printf("First Fit Allocation:\n");
    firstFit();
    printAllocation();
    resetBlockSizes();

    printf("\nBest Fit Allocation:\n");
    bestFit();
    printAllocation();
    resetBlockSizes();

    printf("\nWorst Fit Allocation:\n");
    worstFit();
    printAllocation();

    return 0;
}
