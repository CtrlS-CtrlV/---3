
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Використання: %s <джерело> <призначення>\n", argv[0]);
        return 1;
    }

    char *src = argv[1];
    char *dst = argv[2];

    int fd_src = open(src, O_RDONLY);
    if (fd_src == -1) {
        perror("Помилка відкриття вихідного файлу");
        return 1;
    }

    int fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst == -1) {
        perror("Помилка створення файлу призначення");
        close(fd_src);
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t n_read;
    long long total_bytes = 0;
    int iterations = 0;

    while ((n_read = read(fd_src, buf, BUF_SIZE)) > 0) {
        iterations++;
        ssize_t n_written = 0;
        
        while (n_written < n_read) {
            ssize_t w = write(fd_dst, buf + n_written, n_read - n_written);
            if (w == -1) {
                perror("Помилка запису");
                close(fd_src);
                close(fd_dst);
                return 1;
            }
            n_written += w;
        }
        total_bytes += n_read;
    }

    if (n_read == -1) {
        perror("Помилка читання");
    }

    printf("Скопійовано успішно!\n");
    printf("Кількість прочитаних блоків (ітерацій): %d\n", iterations);
    if (iterations > 0) {
        printf("Середній розмір блоку: %lld байт\n", total_bytes / iterations);
    }
    printf("Загальний розмір: %lld байт\n", total_bytes);

    close(fd_src);
    close(fd_dst);
    return 0;
}
