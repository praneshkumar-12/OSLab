#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

struct Process {
    int pid;
    int arrival;
    int burst;
    int start;
    int finish;
    int wait;
    int turnaround;
    bool completed;
};

void sortByArrival(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival > p[j + 1].arrival) {
                const struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void sortByFinish(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].finish > p[j + 1].finish) {
                const struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

int findNextProcess(struct Process p[], int n, int currentTime) {
    int shortest = -1;
    int minBurst = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= currentTime && !p[i].completed && p[i].burst < minBurst) {
            minBurst = p[i].burst;
            shortest = i;
        }
    }

    return shortest;
}

void calculateTimes(struct Process p[], int n) {
    int currentTime = 0;
    int completedProcesses = 0;

    while (completedProcesses < n) {
        int nextProcess = findNextProcess(p, n, currentTime);

        if (nextProcess == -1) {
            currentTime++;
        } else {
            struct Process *proc = &p[nextProcess];
            proc->start = currentTime;
            proc->finish = proc->start + proc->burst;
            proc->turnaround = proc->finish - proc->arrival;
            proc->wait = proc->turnaround - proc->burst;
            proc->completed = true;

            currentTime = proc->finish;
            completedProcesses++;
        }
    }
}

void printGanttChart(struct Process p[], int n) {

    sortByFinish(p, n);
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf(" P%d", p[i].pid);
        if (i != n - 1) {
            printf("|");
        }
    }
    printf("\n");
    printf("%d", p[0].start);

    for (int i = 0; i < n; i++) {
        printf("    %d", p[i].finish);
    }
    printf("\n");
}

void printTable(struct Process p[], int n) {
    printf("\nProcess\tArrival\tBurst\tStart\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst, p[i].start, p[i].finish, p[i].turnaround, p[i].wait);
    }
}

int main() {
    printf("Enter the number of processes: ");
    int n;
    scanf("%d", &n);
    struct Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time of process %d: ", i + 1);
        scanf("%d", &p[i].arrival);
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &p[i].burst);
        p[i].pid = i + 1;
        p[i].completed = false;
    }

    sortByArrival(p, n);

    calculateTimes(p, n);

    printGanttChart(p, n);

    printTable(p, n);
}
