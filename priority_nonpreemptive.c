#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;        // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int priority;   // Priority (lower number = higher priority)
    int finish;     // Finish time
    int wait;       // Waiting time
    int turnaround; // Turnaround time
    int completed;  // Completion flag
};

// Function to find the process with the highest priority at the current time
int findNextProcess(struct Process p[], int n, int currentTime) {
    int highestPriority = INT_MAX; // A large number for priority comparison
    int index = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= currentTime && p[i].completed == 0) {
            if (p[i].priority < highestPriority) {
                highestPriority = p[i].priority;
                index = i;
            }
            // If two processes have the same priority, choose the one that arrived first
            else if (p[i].priority == highestPriority) {
                if (p[i].arrival < p[index].arrival) {
                    index = i;
                }
            }
        }
    }

    return index;
}

// Function to print the process table
void printProcessTable(struct Process p[], int n) {
    printf("\nProcess\tArrival\tBurst\tPriority\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\t\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst, p[i].priority, p[i].finish, p[i].turnaround, p[i].wait);
    }
}

void printGanttChart(int gantt_start[], int gantt_process[], int gantt_index) {
    printf("\nGantt Chart:\n");

    for (int i = 0; i < gantt_index; i++) {
        printf(" P%d", gantt_process[i]);
        if (i != gantt_index - 1) {
            printf("|");
        }
    }

    printf("\n%d", gantt_start[0]);
    for (int i = 1; i <= gantt_index; i++) {
        printf("    %d", gantt_start[i]);
    }
    printf("\n");
}

// Function to execute the Priority Scheduling (Non-Preemptive)
void executePriorityScheduling(struct Process p[], int n) {
    int currentTime = 0; // Current time
    int completed = 0;   // Number of completed processes

    int gantt_start[100], gantt_process[100], gantt_index = 0;

    while (completed < n) {
        // Find the next process with the highest priority
        int index = findNextProcess(p, n, currentTime);

        if (index != -1) {
            // Execute the process
            if (currentTime < p[index].arrival){
                currentTime = p[index].arrival;
            }
            if (gantt_index == 0 || gantt_process[gantt_index - 1] != p[index].pid) {
                gantt_start[gantt_index] = currentTime;
                gantt_process[gantt_index++] = p[index].pid;
            }

            currentTime += p[index].burst;

            p[index].finish = currentTime;
            p[index].turnaround = p[index].finish - p[index].arrival;
            p[index].wait = p[index].turnaround - p[index].burst;
            p[index].completed = 1; // Mark process as completed

            completed++;
        } else {
            // If no process is available, advance time
            currentTime++;
        }
    }
    gantt_start[gantt_index] = currentTime;

    printGanttChart(gantt_start, gantt_process, gantt_index);
}

int main() {
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // Input for processes
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time, burst time, and priority for process P%d: ", p[i].pid);
        scanf("%d %d %d", &p[i].arrival, &p[i].burst, &p[i].priority);
        p[i].completed = 0; // Initially, no process is completed
    }

    // Execute the Priority Scheduling algorithm (Non-Preemptive)
    executePriorityScheduling(p, n);

    printProcessTable(p, n);

    return 0;
}
