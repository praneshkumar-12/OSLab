#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list_directory(const char *path, int level) {
    struct dirent *entry;
    struct stat info;

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' ||
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }

        // Construct the full path
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        // Get information about the entry
        if (stat(fullPath, &info) == 0) {
            // Print indentation based on the level of recursion
            for (int i = 0; i < level; i++) {
                printf("    ");
            }

            // Print entry type (directory or file) and name
            if (S_ISDIR(info.st_mode)) {
                printf("├── [DIR]  %s\n", entry->d_name);
                // Recur into the subdirectory
                list_directory(fullPath, level + 1);
            } else {
                printf("├── [FILE] %s\n", entry->d_name);
            }
        } else {
            perror("Unable to get file status");
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    printf("Directory tree of '%s':\n", argv[1]);
    list_directory(argv[1], 0);
    return 0;
}
