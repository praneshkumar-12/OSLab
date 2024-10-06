#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int burst;
    int arrival;
    int wait;
    int turnaround;
    int start;
    int finish;
    int is_completed;
    int remaining;
};

int findShortestProcess(struct Process p[], int n, int currentTime) {
    int shortest = -1;
    int minBurst = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= currentTime && p[i].remaining > 0 && p[i].remaining < minBurst) {
            minBurst = p[i].remaining;
            shortest = i;
        }
    }

    return shortest;
}

void executeProcess(struct Process p[], int n, int* gantt_start, int* gantt_process, int* gantt_index) {
    int currentTime = 0;
    int completedProcesses = 0;
    int lastProcess = -1;

    while (completedProcesses < n) {
        int shortest = findShortestProcess(p, n, currentTime);

        if (shortest == -1) {
            currentTime++;
        }
        else {
            if (shortest != lastProcess) {
                gantt_start[*gantt_index] = currentTime;
                gantt_process[*gantt_index] = p[shortest].pid;
                (*gantt_index)++;
                lastProcess = shortest;
            }

            currentTime++;
            p[shortest].remaining--;

            if (p[shortest].remaining == 0) {
                completedProcesses++;
                p[shortest].finish = currentTime;
                p[shortest].is_completed = 1;
                p[shortest].turnaround = p[shortest].finish - p[shortest].arrival;
                p[shortest].wait = p[shortest].turnaround - p[shortest].burst;
                lastProcess = -1;
            }
        }
    }

    gantt_start[*gantt_index] = currentTime;
}

void printGanttChart(int gantt_start[], int gantt_process[], int gantt_index){
    printf("Gantt Chart:\n");

    for (int i = 0; i < gantt_index; i++){
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
    printf("\nProcess\tArrival\tBurst\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst, p[i].finish, p[i].turnaround, p[i].wait);
    }
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    int gantt_start[n*2], gantt_process[n*2], gantt_index = 0;

    for (int i = 0; i < n; i++) {
        printf("Enter the arrival time of process %d: ", i+1);
        scanf("%d", &p[i].arrival);
        printf("Enter the burst time of process %d: ", i+1);
        scanf("%d", &p[i].burst);
        p[i].pid = i+1;
        p[i].is_completed = 0;
        p[i].remaining = p[i].burst;
    }

    executeProcess(p, n, gantt_start, gantt_process, &gantt_index);

    printGanttChart(gantt_start, gantt_process, gantt_index);

    printProcessTable(p, n);
}