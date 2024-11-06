#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of directories and files
#define MAX_DIRS 100
#define MAX_FILES 100

// Structure for a directory
typedef struct Directory {
    char name[50];
    struct Directory *subDirs[MAX_DIRS];
    int subDirCount;
    struct File *files[MAX_FILES];
    int fileCount;
    struct Directory *link; // Directory shortcut (link)
} Directory;

// Structure for a file
typedef struct File {
    char name[50];
    int size;
} File;

// Function to create a new directory
Directory *createDirectory(char *name) {
    Directory *newDir = (Directory *)malloc(sizeof(Directory));
    strcpy(newDir->name, name);
    newDir->subDirCount = 0;
    newDir->fileCount = 0;
    newDir->link = NULL;
    return newDir;
}

// Function to create a new file
File *createFile(char *name, int size) {
    File *newFile = (File *)malloc(sizeof(File));
    strcpy(newFile->name, name);
    newFile->size = size;
    return newFile;
}

// Function to add a sub-directory to a directory
void addSubDirectory(Directory *parent, Directory *subDir) {
    if (parent->subDirCount < MAX_DIRS) {
        parent->subDirs[parent->subDirCount++] = subDir;
    } else {
        printf("Maximum subdirectories reached in %s.\n", parent->name);
    }
}

// Function to add a file to a directory
void addFile(Directory *dir, File *file) {
    if (dir->fileCount < MAX_FILES) {
        dir->files[dir->fileCount++] = file;
    } else {
        printf("Maximum files reached in %s.\n", dir->name);
    }
}

// Function to create a shortcut (link) to a directory
void createLink(Directory *source, Directory *target) {
    source->link = target;
}

// Function to display directory contents
void displayDirectory(Directory *dir, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("Directory: %s\n", dir->name);
    
    // Display files
    for (int i = 0; i < dir->fileCount; i++) {
        for (int j = 0; j < level + 1; j++) {
            printf("  ");
        }
        printf("File: %s (Size: %d bytes)\n", dir->files[i]->name, dir->files[i]->size);
    }

    // Display subdirectories
    for (int i = 0; i < dir->subDirCount; i++) {
        displayDirectory(dir->subDirs[i], level + 1);
    }

    // Display link if any
    if (dir->link != NULL) {
        for (int i = 0; i < level + 1; i++) {
            printf("  ");
        }
        printf("Shortcut Link to: %s\n", dir->link->name);
    }
}

// Function to free directory and its contents
void freeDirectory(Directory *dir) {
    for (int i = 0; i < dir->fileCount; i++) {
        free(dir->files[i]);
    }
    for (int i = 0; i < dir->subDirCount; i++) {
        freeDirectory(dir->subDirs[i]);
    }
    free(dir);
}

int main() {
    Directory *root = createDirectory("Root");
    Directory *currentDir = root;
    Directory *newDir;
    File *newFile;
    int choice, size;
    char name[50];

    while (1) {
        // Display Menu
        printf("\nMenu:\n");
        printf("1. Create Directory\n");
        printf("2. Add File to Directory\n");
        printf("3. Create Link to Directory\n");
        printf("4. Display Directory Structure\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:  // Create Directory
                printf("Enter directory name: ");
                scanf("%s", name);
                newDir = createDirectory(name);
                addSubDirectory(currentDir, newDir);
                printf("Directory '%s' created and added to '%s'.\n", name, currentDir->name);
                break;

            case 2:  // Add File
                printf("Enter file name: ");
                scanf("%s", name);
                printf("Enter file size (in bytes): ");
                scanf("%d", &size);
                newFile = createFile(name, size);
                addFile(currentDir, newFile);
                printf("File '%s' of size %d bytes added to '%s'.\n", name, size, currentDir->name);
                break;

            case 3:  // Create Link
                printf("Enter source directory name: ");
                scanf("%s", name);
                Directory *source = NULL;
                for (int i = 0; i < currentDir->subDirCount; i++) {
                    if (strcmp(currentDir->subDirs[i]->name, name) == 0) {
                        source = currentDir->subDirs[i];
                        break;
                    }
                }
                if (source == NULL) {
                    printf("Directory not found.\n");
                    break;
                }

                printf("Enter target directory name: ");
                scanf("%s", name);
                Directory *target = NULL;
                for (int i = 0; i < currentDir->subDirCount; i++) {
                    if (strcmp(currentDir->subDirs[i]->name, name) == 0) {
                        target = currentDir->subDirs[i];
                        break;
                    }
                }
                if (target == NULL) {
                    printf("Directory not found.\n");
                    break;
                }

                createLink(source, target);
                printf("Link created from '%s' to '%s'.\n", source->name, target->name);
                break;

            case 4:  // Display Directory Structure
                displayDirectory(currentDir, 0);
                break;

            case 5:  // Exit
                freeDirectory(root);
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice, please try again.\n");
        }
    }
}
