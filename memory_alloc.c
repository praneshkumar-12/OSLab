#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 5
#define MAX_BLOCKS 20

// File structure
typedef struct {
    char name[50];
    int size;  // Number of blocks the file occupies
    int *blocks;  // Array to hold block indexes (for linked/indexed allocation)
} File;

// Memory structure for each allocation technique
typedef struct {
    int *memory;  // Array to simulate memory blocks
    int total_blocks;
    int free_blocks;
} Memory;

// Function to initialize memory for Contiguous Allocation
void initContiguousMemory(Memory *mem) {
    mem->total_blocks = MAX_BLOCKS;
    mem->free_blocks = MAX_BLOCKS;
    mem->memory = (int *)calloc(mem->total_blocks, sizeof(int));  // 0 for free, 1 for allocated
}

// Function to initialize memory for Linked Allocation
void initLinkedMemory(Memory *mem) {
    mem->total_blocks = MAX_BLOCKS;
    mem->free_blocks = MAX_BLOCKS;
    mem->memory = (int *)malloc(mem->total_blocks * sizeof(int));  // 0 for free, 1 for allocated
    for (int i = 0; i < MAX_BLOCKS - 1; i++) {
        mem->memory[i] = i + 1;  // Point to the next block in linked allocation
    }
    mem->memory[MAX_BLOCKS - 1] = -1;  // End of list
}

// Function to initialize memory for Indexed Allocation
void initIndexedMemory(Memory *mem) {
    mem->total_blocks = MAX_BLOCKS;
    mem->free_blocks = MAX_BLOCKS;
    mem->memory = (int *)calloc(mem->total_blocks, sizeof(int));  // 0 for free, 1 for allocated
}

// Function to allocate memory using contiguous allocation
int allocateContiguousMemory(Memory *mem, File *file) {
    for (int i = 0; i < mem->total_blocks - file->size + 1; i++) {
        int can_allocate = 1;
        for (int j = 0; j < file->size; j++) {
            if (mem->memory[i + j] == 1) {
                can_allocate = 0;
                break;
            }
        }
        if (can_allocate) {
            for (int j = 0; j < file->size; j++) {
                mem->memory[i + j] = 1;  // Mark blocks as allocated
            }
            file->blocks = (int *)malloc(file->size * sizeof(int));
            for (int j = 0; j < file->size; j++) {
                file->blocks[j] = i + j;
            }
            mem->free_blocks -= file->size;
            return 1;
        }
    }
    return 0;  // Not enough contiguous blocks
}

// Function to allocate memory using linked allocation
int allocateLinkedMemory(Memory *mem, File *file) {
    if (mem->free_blocks < file->size) {
        return 0;  // Not enough free blocks
    }
    file->blocks = (int *)malloc(file->size * sizeof(int));
    int prev_block = -1;
    int start_block = -1;
    for (int i = 0; i < mem->total_blocks; i++) {
        if (mem->memory[i] == 0) {
            if (start_block == -1) {
                start_block = i;
            }
            file->blocks[i] = i;
            if (prev_block != -1) {
                mem->memory[prev_block] = i;  // Linking blocks
            }
            prev_block = i;
        }
    }
    mem->memory[prev_block] = -1;  // End of list
    mem->free_blocks -= file->size;
    return 1;
}

// Function to allocate memory using indexed allocation
int allocateIndexedMemory(Memory *mem, File *file) {
    if (mem->free_blocks < file->size) {
        return 0;  // Not enough free blocks
    }
    file->blocks = (int *)malloc(file->size * sizeof(int));
    int block_count = 0;
    for (int i = 0; i < mem->total_blocks && block_count < file->size; i++) {
        if (mem->memory[i] == 0) {
            file->blocks[block_count++] = i;
            mem->memory[i] = 1;
        }
    }
    mem->free_blocks -= file->size;
    return 1;
}

// Function to deallocate memory for a file
void deallocateMemory(Memory *mem, File *file) {
    for (int i = 0; i < file->size; i++) {
        mem->memory[file->blocks[i]] = 0;  // Mark the blocks as free
    }
    free(file->blocks);
    mem->free_blocks += file->size;
}

// Function to display memory status
void displayMemoryStatus(Memory *mem) {
    printf("Memory Status: ");
    for (int i = 0; i < mem->total_blocks; i++) {
        printf("%d ", mem->memory[i]);
    }
    printf("\n");
}

int main() {
    Memory mem;
    File files[MAX_FILES];
    int file_count = 0;

    int choice;
    int allocation_type = 0; // 1 - Contiguous, 2 - Linked, 3 - Indexed
    
    while (1) {
        // Display Menu
        printf("\nMenu:\n");
        printf("1. Select Allocation Type\n");
        printf("2. Create File\n");
        printf("3. Deallocate File\n");
        printf("4. Display Memory Status\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nSelect Allocation Type:\n");
                printf("1. Contiguous Allocation\n");
                printf("2. Linked Allocation\n");
                printf("3. Indexed Allocation\n");
                printf("Enter choice: ");
                scanf("%d", &allocation_type);
                
                if (allocation_type == 1) {
                    initContiguousMemory(&mem);
                    printf("Contiguous Memory Allocation selected.\n");
                } else if (allocation_type == 2) {
                    initLinkedMemory(&mem);
                    printf("Linked Memory Allocation selected.\n");
                } else if (allocation_type == 3) {
                    initIndexedMemory(&mem);
                    printf("Indexed Memory Allocation selected.\n");
                }
                break;

            case 2:
                if (file_count < MAX_FILES) {
                    printf("Enter file name: ");
                    scanf("%s", files[file_count].name);
                    printf("Enter file size (number of blocks): ");
                    scanf("%d", &files[file_count].size);
                    
                    int allocated = 0;
                    if (allocation_type == 1) {
                        allocated = allocateContiguousMemory(&mem, &files[file_count]);
                    } else if (allocation_type == 2) {
                        allocated = allocateLinkedMemory(&mem, &files[file_count]);
                    } else if (allocation_type == 3) {
                        allocated = allocateIndexedMemory(&mem, &files[file_count]);
                    }

                    if (allocated) {
                        printf("File '%s' allocated successfully.\n", files[file_count].name);
                        file_count++;
                    } else {
                        printf("Not enough memory to allocate file '%s'.\n", files[file_count].name);
                    }
                } else {
                    printf("Maximum file limit reached.\n");
                }
                break;

            case 3:
                {
                    char filename[50];
                    printf("Enter the file name to deallocate: ");
                    scanf("%s", filename);
                    
                    int found = 0;
                    for (int i = 0; i < file_count; i++) {
                        if (strcmp(files[i].name, filename) == 0) {
                            deallocateMemory(&mem, &files[i]);
                            printf("File '%s' deallocated successfully.\n", files[i].name);
                            found = 1;
                            // Shift the remaining files
                            for (int j = i; j < file_count - 1; j++) {
                                files[j] = files[j + 1];
                            }
                            file_count--;
                            break;
                        }
                    }
                    if (!found) {
                        printf("File not found.\n");
                    }
                }
                break;

            case 4:
                displayMemoryStatus(&mem);
                break;

            case 5:
                printf("Exiting...\n");
                free(mem.memory);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
