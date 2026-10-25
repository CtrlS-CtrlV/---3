#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void print_spaces(int depth)
{
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}

void walk(const char *dir_path, int depth, int max_depth, int *files, int *dirs, long long *size)
{
    if (depth > max_depth) return;

    DIR *dir = opendir(dir_path);
    if (dir == NULL) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == -1) {
            continue;
        }

        print_spaces(depth);

        if (S_ISDIR(st.st_mode)) {
            printf("[DIR]  %s/\n", entry->d_name);
            (*dirs)++;
            walk(full_path, depth + 1, max_depth, files, dirs, size);
        } else if (S_ISREG(st.st_mode)) {
            printf("[FILE] %s (%ld байт)\n", entry->d_name, (long)st.st_size);
            (*files)++;
            *size += st.st_size;
        } else if (S_ISLNK(st.st_mode)) {
            char target[256];
            ssize_t len = readlink(full_path, target, sizeof(target) - 1);
            if (len != -1) {
                target[len] = '\0';
                printf("[LINK] %s -> %s\n", entry->d_name, target);
            } else {
                printf("[LINK] %s\n", entry->d_name);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    char *path = (argc > 1) ? argv[1] : ".";
    int max_depth = (argc > 2) ? atoi(argv[2]) : 2;

    int files = 0, dirs = 0;
    long long total_size = 0;

    printf("Дерево каталогу: %s (глибина: %d)\n", path, max_depth);
    printf("========================================\n");

    walk(path, 0, max_depth, &files, &dirs, &total_size);

    printf("========================================\n");
    printf("Підсумок:\n");
    printf("Каталогів:       %d\n", dirs);
    printf("Файлів:          %d\n", files);
    printf("Загальний розмір: %lld байт\n", total_size);

    return 0;
}
