#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int fd1[2], fd2[2];
    int st;
    pid_t pid;

    if (argc != 4) {
        fprintf(stderr, "wrong number of parameters\n");
        return 0;
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    //c
    if ((pid = fork()) == 0) {
        dup2(fd2[0], 0);
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        close(fd1[1]);
        execlp(argv[3], argv[3], NULL);
        fprintf(stderr, "exec in c failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "fork failed\n");
        exit(0);
    }

    //b
    if ((pid = fork()) == 0) {
        dup2(fd2[1], 1);
        dup2(fd2[1], 2);
        dup2(fd1[0], 0);
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        close(fd1[1]);
        execlp(argv[2], argv[2], NULL);
        fprintf(stderr, "exec in pr2 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "third fork failed\n");
        exit(0);
    }

    //a
    if ((pid = fork()) == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec in b failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "fork failed\n");
        exit(0);
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    wait(&st);
    wait(&st);
    wait(&st);

    return 0;
}
