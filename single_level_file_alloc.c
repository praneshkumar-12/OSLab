#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

void list_directory(const char *path) {
    struct dirent *entry;
    
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

        printf("%s\n", entry->d_name);
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
