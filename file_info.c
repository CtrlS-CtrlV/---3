
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void get_mode_str(mode_t mode, char *str)
{
    str[0] = S_ISDIR(mode) ? 'd' : (S_ISLNK(mode) ? 'l' : '-');
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';
}

void print_info(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("Помилка stat");
        return;
    }

    char mode_str[12];
    get_mode_str(st.st_mode, mode_str);

    printf("\nІнформація про файл: %s\n", path);
    printf("Символьні права: %s\n", mode_str);
    printf("Числові права:   %04o\n", st.st_mode & 0777);
    printf("Розмір файлу:    %ld байтів\n", (long)st.st_size);

    struct passwd *pw = getpwuid(st.st_uid);
    struct group *gr = getgrgid(st.st_gid);
    printf("Власник:         %s\n", pw ? pw->pw_name : "невідомо");
    printf("Група:           %s\n", gr ? gr->gr_name : "невідомо");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Використання: %s <файл> [нові_права_у_вісімковій_системі]\n", argv[0]);
        return 1;
    }

    printf("--- ДО ЗМІНИ ПРАВ ---");
    print_info(argv[1]);

    if (argc >= 3) {
        mode_t new_mode = (mode_t)strtol(argv[2], NULL, 8);
        
        if (chmod(argv[1], new_mode) == -1) {
            perror("Помилка chmod");
            return 1;
        }

        printf("\nПрава успішно змінено на %04o!\n", new_mode);
        
        printf("--- ПІСЛЯ ЗМІНИ ПРАВ ---");
        print_info(argv[1]);
    }

    return 0;
}
