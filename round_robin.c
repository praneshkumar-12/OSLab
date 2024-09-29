#include <stdio.h>

struct Process {
    int pid;
    int arrival;
    int burst;
    int wait;
    int turnaround;
    int start;
    int finish;
    int remaining;
};

void sortByArrival(struct Process p[], int n){
    for (int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - 1; j++){
            if (p[j].arrival > p[j+1].arrival){
                struct Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
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

void printProcessTable(struct Process p[], int n) {
    printf("\nProcess\tArrival\tBurst\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst, p[i].finish, p[i].turnaround, p[i].wait);
    }
}

void executeRoundRobin(struct Process p[], int n, int timeQuantum) {
    int currentTime = 0;
    int completedProcesses = 0;
    int gantt_index = 0;

    int gantt_start[100], gantt_process[100];

    while (completedProcesses < n) {
        int found = 0;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= currentTime && p[i].remaining > 0) {

                found = 1;

                if (gantt_index == 0 || gantt_process[gantt_index - 1] != p[i].pid) {
                    gantt_start[gantt_index] = currentTime;
                    gantt_process[gantt_index++] = p[i].pid;
                }

                if (p[i].remaining > timeQuantum) {
                    currentTime += timeQuantum;
                    p[i].remaining -= timeQuantum;
                } else {
                    completedProcesses++;
                    currentTime += p[i].remaining;
                    p[i].finish = currentTime;
                    p[i].turnaround = p[i].finish - p[i].arrival;
                    p[i].wait = p[i].turnaround - p[i].burst;
                    p[i].remaining = 0;
                }
            }
        }
        if (!found) {
            currentTime++;
        }
    }
        gantt_start[gantt_index] = currentTime;
        printGanttChart(gantt_start, gantt_process, gantt_index);
}

int main() {
    int n;
    int timeQuantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct Process p[n];

    for (int i = 0; i < n; i++) {
        printf("Enter the arrival time: ");
        scanf("%d", &p[i].arrival);
        printf("Enter the burst time: ");
        scanf("%d", &p[i].burst);
        p[i].pid = i + 1;
        p[i].remaining = p[i].burst;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    sortByArrival(p, n);

    executeRoundRobin(p, n, timeQuantum);

    printProcessTable(p, n);
}