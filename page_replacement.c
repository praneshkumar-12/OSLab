#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PAGES 100
#define MAX_FRAMES 10

int pageReference[MAX_PAGES];
int frames[MAX_FRAMES];

// Function to check if a page is already in frames
bool isPageInFrames(int frames[], int numFrames, int page) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

// Function to print current frames
void printFrames(int frames[], int numFrames) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == -1) {
            printf("- ");
        } else {
            printf("%d ", frames[i]);
        }
    }
    printf("\n");
}

// FIFO Page Replacement
void fifoPageReplacement(int numPages, int numFrames) {
    int fifoIndex = 0, pageFaults = 0;

    // Initialize frames
    for (int i = 0; i < numFrames; i++) frames[i] = -1;

    printf("\nFIFO Page Replacement Simulation:\n");
    for (int i = 0; i < numPages; i++) {
        int page = pageReference[i];
        printf("Page %d -> Frames: ", page);
        if (!isPageInFrames(frames, numFrames, page)) {
            frames[fifoIndex] = page;
            fifoIndex = (fifoIndex + 1) % numFrames;
            pageFaults++;
        }
        printFrames(frames, numFrames);
    }
    printf("Total Page Faults: %d\n", pageFaults);
}

// LRU Page Replacement
void lruPageReplacement(int numPages, int numFrames) {
    int pageFaults = 0;

    // Initialize frames
    for (int i = 0; i < numFrames; i++) frames[i] = -1;

    printf("\nLRU Page Replacement Simulation:\n");
    for (int i = 0; i < numPages; i++) {
        int page = pageReference[i];
        printf("Page %d -> Frames: ", page);
        if (!isPageInFrames(frames, numFrames, page)) {
            int leastUsedIndex = 0, leastUsedValue = INT_MAX;

            // Find the least recently used page
            for (int j = 0; j < numFrames; j++) {
                if (frames[j] == -1) {
                    leastUsedIndex = j;
                    break;
                }
                int lastUsed = -1;
                for (int k = i - 1; k >= 0; k--) {
                    if (pageReference[k] == frames[j]) {
                        lastUsed = k;
                        break;
                    }
                }
                if (lastUsed < leastUsedValue) {
                    leastUsedValue = lastUsed;
                    leastUsedIndex = j;
                }
            }
            frames[leastUsedIndex] = page;
            pageFaults++;
        }
        printFrames(frames, numFrames);
    }
    printf("Total Page Faults: %d\n", pageFaults);
}

// Optimal Page Replacement
void optimalPageReplacement(int numPages, int numFrames) {
    int pageFaults = 0;

    // Initialize frames
    for (int i = 0; i < numFrames; i++) frames[i] = -1;

    printf("\nOptimal Page Replacement Simulation:\n");
    for (int i = 0; i < numPages; i++) {
        int page = pageReference[i];
        printf("Page %d -> Frames: ", page);
        if (!isPageInFrames(frames, numFrames, page)) {
            int replaceIndex = 0, furthestUse = -1;

            // Find the page to be replaced
            for (int j = 0; j < numFrames; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }
                int nextUse = INT_MAX;
                for (int k = i + 1; k < numPages; k++) {
                    if (pageReference[k] == frames[j]) {
                        nextUse = k;
                        break;
                    }
                }
                if (nextUse > furthestUse) {
                    furthestUse = nextUse;
                    replaceIndex = j;
                }
            }
            frames[replaceIndex] = page;
            pageFaults++;
        }
        printFrames(frames, numFrames);
    }
    printf("Total Page Faults: %d\n", pageFaults);
}

int main() {
    int numPages, numFrames;

    printf("Enter the number of pages: ");
    scanf("%d", &numPages);

    printf("Enter the page reference string:\n");
    for (int i = 0; i < numPages; i++) {
        scanf("%d", &pageReference[i]);
    }

    printf("Enter the number of frames: ");
    scanf("%d", &numFrames);

    // Simulate each page replacement algorithm
    fifoPageReplacement(numPages, numFrames);
    lruPageReplacement(numPages, numFrames);
    optimalPageReplacement(numPages, numFrames);

    return 0;
}

// LRU Page Replacement Simulation:
// Page 4 -> Frames: 4 - -
// Page 2 -> Frames: 4 2 -
// Page 1 -> Frames: 4 2 1
// Page 4 -> Frames: 4 2 1
// Page 3 -> Frames: 4 3 1
// Page 2 -> Frames: 4 3 2
// Page 1 -> Frames: 1 3 2
// Page 5 -> Frames: 1 5 2
// Total Page Faults: 7

// Enter the number of pages: 8
// Enter the page reference string:
// 4 2 1 4 3 2 1 5
// Enter the number of frames: 3