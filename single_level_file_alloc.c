#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

void list_directory(const char *path) {
    struct dirent *entry;
    struct stat info;
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    printf("Contents of directory '%s':\n", path);

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || 
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }

        // Get the path for each entry
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        // Get information about the entry
        if (stat(fullPath, &info) == 0) {
            if (S_ISDIR(info.st_mode)) {
                printf("├── [DIR]  %s\n", entry->d_name);
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

    list_directory(argv[1]);
    return 0;
}
