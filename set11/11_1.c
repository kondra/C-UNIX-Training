#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pid, fd, st;

    if ((pid = fork()) == 0) {
        if ((fd = creat(argv[3], 0666)) < 0) {
            printf("creat failed\n");
            exit(0);
        }
        dup2(fd, 2);
        close(fd);
        execlp(argv[1], argv[1], NULL);
        printf("exec failed\n");
    } else if (pid < 0) {
        printf("fork failed\n");
        exit(0);
    }

    wait(&st);

    if (WIFEXITED(st) && !WEXITSTATUS(st)) {
        if ((pid = fork()) == 0) {
            if ((fd = open(argv[3], O_RDONLY)) < 0) {
                printf("open failed\n");
                exit(0);
            }
            dup2(fd, 0);
            close(fd);
            execlp(argv[2], argv[2], NULL);
            printf("exec failed\n");
        } else if (pid < 0) {
            printf("fork failed\n");
            exit(0);
        }
    }

    wait(&st);

    return 0;
}
