# include <stdio.h>

struct Process {
    int pid;
    int arrival;
    int burst;
    int wait;
    int start;
    int finish;
    int turnaround;
};


void sortByArrivalTime(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (p[j].arrival > p[j + 1].arrival) {
                const struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void calculateTime(struct Process  p[], int n) {
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival > currentTime) {
            currentTime = p[i].arrival;
        }

        p[i].start = currentTime;
        p[i].finish = p[i].start + p[i].burst;
        p[i].turnaround = p[i].finish - p[i].arrival;
        p[i].wait = p[i].turnaround - p[i].burst;
        currentTime = p[i].finish;
    }
}

void printGanttChart(struct Process p[], int n) {
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf(" P%d", i+1);
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
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) {
        printf("Enter the arrival time of process %d: ", i + 1);
        scanf("%d", &p[i].arrival);
        printf("Enter the burst time of process %d: ", i + 1);
        scanf("%d", &p[i].burst);
        p[i].pid = i + 1;
    }

    sortByArrivalTime(p, n);

    calculateTime(p, n);

    printGanttChart(p, n);

    printTable(p, n);
}