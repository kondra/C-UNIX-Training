#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int **fd;
    int k, n, i, j;
    int pid, st, s;
    int num = 0;

    if (argc < 2) {
        printf("wrong parameters\n");
        return 0;
    }

    k = strtol(argv[1], NULL, 10);
    n = strtol(argv[2], NULL, 10);

    if (k == 0 || k == 1) {
        printf("k should be >= 2\n");
        return 0;
    }
    if (n == 0) {
        printf("0 - 0\n");
        return 0;
    }

    fd = (int**) malloc(sizeof (int*) * k);
    for (i = 0; i < k; i++) {
        fd[i] = (int*) malloc(sizeof (int) * 2);
        s = pipe(fd[i]);
        if (s < 0) {
            printf("%d pipe failed\n", i);
            return 0;
        }
    }

    for (i = 1; i < k; i++) {
        if ((pid = fork()) == 0) {
            for (j = 0; j < k; j++) {
                if (i == j) {
                    close(fd[i][0]);
                    close(fd[i - 1][1]);
                    continue;
                }
                if (j == i - 1)
                    continue;
                close(fd[j][0]);
                close(fd[j][1]);
            }
            while ((s = read(fd[i - 1][0], &num, sizeof (int))) == sizeof (int)) {
                printf("%d - %d\n", i, num);
                num++;
                if (n == num)
                    break;
                s = write(fd[i][1], &num, sizeof (int));
            }
            close(fd[i][1]);
            close(fd[i - 1][0]);
            exit(0);
        } else if (pid < 0) {
            printf("%d fork failed\n", i);
            return 0;
        }
    }

    for (i = 1; i < k - 1; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }
    close(fd[0][0]);
    close(fd[k - 1][1]);

    s = write(fd[0][1], &num, sizeof (int));
    while ((s = read(fd[k - 1][0], &num, sizeof (int))) == sizeof (int)) {
        printf("0 - %d\n", num);
        num++;
        if (num == n)
            break;
        s = write(fd[0][1], &num, sizeof (int));
    }
    close(fd[0][1]);
    close(fd[k - 1][0]);

    for (i = 0; i < k; i++)
        wait(&st);

    return 0;
}
