
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char *path = (argc > 1) ? argv[1] : ".";

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Помилка opendir");
        return 1;
    }

    printf("\nВміст директорії '%s':\n", path);
    printf("%-10s %-10s %s\n", "Тип", "Розмір", "Ім'я");
    printf("------------------------------------\n");

    struct dirent *entry;
    int files_count = 0;
    int dirs_count = 0;
    long long total_size = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == -1) {
            continue;
        }

        if (S_ISREG(st.st_mode)) {
            printf("%-10s %-10ld %s\n", "[FILE]", (long)st.st_size, entry->d_name);
            files_count++;
            total_size += st.st_size;
        } else if (S_ISDIR(st.st_mode)) {
            printf("%-10s %-10s %s\n", "[DIR]", "<DIR>", entry->d_name);
            dirs_count++;
        } else {
            printf("%-10s %-10s %s\n", "[OTHER]", "-", entry->d_name);
        }
    }

    closedir(dir);

    printf("------------------------------------\n");
    printf("Всього файлів:   %d\n", files_count);
    printf("Всього каталогів: %d\n", dirs_count);
    printf("Сумарний розмір:  %lld байт\n", total_size);

    return 0;
}
