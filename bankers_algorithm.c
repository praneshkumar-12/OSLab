#include <stdio.h>
#include <stdbool.h>

#define NUM_PROCESSES 5 // Number of processes
#define NUM_RESOURCES 3 // Number of resource types

// Available resources in the system
int available[NUM_RESOURCES] = {3,3,2};

// Maximum demand of each process
int maximum[NUM_PROCESSES][NUM_RESOURCES] = {
    {7,5,3},
    {3,2,2},
    {9,0,2},
    {2,2,2},
    {4,3,3}
};

// Allocation matrix (resources currently allocated to each process)
int allocation[NUM_PROCESSES][NUM_RESOURCES] = {
    {0,1,0},
    {2,0,0},
    {3,0,2},
    {2,1,1},
    {0,0,2}
};

// Need matrix (remaining resources needed by each process)
int need[NUM_PROCESSES][NUM_RESOURCES];

// Wait queue to store processes that couldn’t execute initially
int waitQueue[NUM_PROCESSES];
int waitQueueCount = 0; // Count of processes in the wait queue

// Function to calculate the Need matrix
void calculateNeed() {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

// Function to check if a process can execute with the available resources
bool canExecute(int process) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (need[process][i] > available[i]) {
            return false;
        }
    }
    return true;
}

// Function to execute a process by releasing its resources
void executeProcess(int process) {
    printf("Executing Process P%d\n", process);
    // Release resources allocated to this process
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] += allocation[process][i];
        allocation[process][i] = 0;
        need[process][i] = 0;
    }
}

// Function to add a process to the wait queue
void addToWaitQueue(int process) {
    waitQueue[waitQueueCount++] = process;
    printf("Process P%d added to wait queue\n", process);
}

// Function to process the wait queue
void processWaitQueue() {
    int i = 0;
    while (i < waitQueueCount) {
        int process = waitQueue[i];
        if (canExecute(process)) {
            executeProcess(process);
            // Shift the remaining elements in the wait queue
            for (int j = i; j < waitQueueCount - 1; j++) {
                waitQueue[j] = waitQueue[j + 1];
            }
            waitQueueCount--; // Reduce wait queue count
        } else {
            i++; // Move to the next process in the wait queue
        }
    }
}

// Main function to execute processes in the original list first, then the wait queue
int main() {
    // Calculate the Need matrix
    calculateNeed();

    // First pass: Try to execute each process in the original list
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (canExecute(i)) {
            executeProcess(i);
        } else {
            addToWaitQueue(i); // Add to wait queue if resources are insufficient
        }
    }

    // Second pass: Process each item in the wait queue
    processWaitQueue();

    // Final status
    if (waitQueueCount == 0) {
        printf("All processes executed successfully.\n");
    } else {
        printf("Some processes could not be executed due to insufficient resources.\n");
    }

    return 0;
}