#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int arrival;
    int burst;
    int start;
    int wait;
    int turnaround;
    int finish;
    int priority;
    int remaining;
};

int findNextProcess(struct Process p[], int n, int currentTime) {
    int highestPriority = INT_MAX;
    int index = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= currentTime && p[i].remaining > 0) {
            if (p[i].priority < highestPriority) {
                highestPriority = p[i].priority;
                index = i;
            } else if (p[i].priority == highestPriority) {
                if (p[i].arrival < p[index].arrival) {
                    index = i;
                }
            }
        }
    }

    return index;

}

void printGanttChart(int gantt_process[], int gantt_start[], int gantt_index) {
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < gantt_index; i++) {
        printf(" P%d ", gantt_process[i]);
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

void printProcessTable(struct Process p[], int n) {
    printf("\nProcess\tArrival\tBurst\tPriority\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\t\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst, p[i].priority, p[i].finish, p[i].turnaround, p[i].wait);
    }
}

void executePriorityScheduling(struct Process p[], int n) {
    int currentTime = 0;
    int completedProcesses = 0;

    int gantt_start[100], gantt_process[100], gantt_index = 0;

    while (completedProcesses < n) {
        int index = findNextProcess(p, n, currentTime);

        if (index == -1) {
            currentTime++;
        } else {
            if (gantt_index == 0 || gantt_process[gantt_index - 1] != p[index].pid) {
                gantt_start[gantt_index] = currentTime;
                gantt_process[gantt_index++] = p[index].pid;
            }

            p[index].remaining--;
            currentTime++;

            if (p[index].remaining == 0) {
                p[index].finish = currentTime;
                p[index].turnaround = p[index].finish - p[index].arrival;
                p[index].wait = p[index].turnaround - p[index].burst;
                completedProcesses++;
            }
        }
    }

    gantt_start[gantt_index] = currentTime;
    printGanttChart(gantt_process, gantt_start, gantt_index);

}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];

    for (int i = 0; i < n; i++) {
        printf("Enter arrival time: ");
        scanf("%d", &p[i].arrival);
        printf("Enter burst time: ");
        scanf("%d", &p[i].burst);
        printf("Enter priority: ");
        scanf("%d", &p[i].priority);
        p[i].pid = i + 1;
        p[i].remaining = p[i].burst;
    }

    executePriorityScheduling(p, n);

    printProcessTable(p, n);
}