#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_FILES 10
#define MAX_NAME_LENGTH 50

// Structure to represent a file
typedef struct {
    char name[MAX_NAME_LENGTH];
} File;

// Structure to represent a user directory
typedef struct {
    char username[MAX_NAME_LENGTH];
    int file_count;
    File files[MAX_FILES];
} UserDirectory;

// Array to store multiple user directories
UserDirectory user_directories[MAX_USERS];
int user_count = 0;

// Function to add a new user
void add_user(const char *username) {
    if (user_count >= MAX_USERS) {
        printf("Maximum user limit reached.\n");
        return;
    }

    strcpy(user_directories[user_count].username, username);
    user_directories[user_count].file_count = 0;
    user_count++;
    printf("User '%s' added successfully.\n", username);
}

// Function to add a file to a user's directory
void add_file(const char *username, const char *filename) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(user_directories[i].username, username) == 0) {
            if (user_directories[i].file_count >= MAX_FILES) {
                printf("Maximum file limit reached for user '%s'.\n", username);
                return;
            }

            strcpy(user_directories[i].files[user_directories[i].file_count].name, filename);
            user_directories[i].file_count++;
            printf("File '%s' added to user '%s'.\n", filename, username);
            return;
        }
    }
    printf("User '%s' not found.\n", username);
}

// Function to list all files of a user
void list_files(const char *username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(user_directories[i].username, username) == 0) {
            printf("Files for user '%s':\n", username);
            for (int j = 0; j < user_directories[i].file_count; j++) {
                printf(" - %s\n", user_directories[i].files[j].name);
            }
            return;
        }
    }
    printf("User '%s' not found.\n", username);
}

// Function to list all users and their files
void list_all_users() {
    if (user_count == 0) {
        printf("No users available.\n");
        return;
    }

    printf("User directories:\n");
    for (int i = 0; i < user_count; i++) {
        printf("User: %s\n", user_directories[i].username);
        if (user_directories[i].file_count == 0) {
            printf("  No files in this directory.\n");
        } else {
            for (int j = 0; j < user_directories[i].file_count; j++) {
                printf("  - %s\n", user_directories[i].files[j].name);
            }
        }
    }
}

int main() {
    int choice;
    char username[MAX_NAME_LENGTH];
    char filename[MAX_NAME_LENGTH];

    while (1) {
        printf("\nTwo-Level File Organization System\n");
        printf("1. Add User\n");
        printf("2. Add File to User\n");
        printf("3. List Files of a User\n");
        printf("4. List All Users and Files\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter username: ");
                scanf("%s", username);
                add_user(username);
                break;

            case 2:
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter filename: ");
                scanf("%s", filename);
                add_file(username, filename);
                break;

            case 3:
                printf("Enter username: ");
                scanf("%s", username);
                list_files(username);
                break;

            case 4:
                list_all_users();
                break;

            case 5:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
