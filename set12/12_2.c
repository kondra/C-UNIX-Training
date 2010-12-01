#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int st, pid, pid3;
    int fd[2], f;

    if (pipe(fd) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(0);
    }

    if ((pid = fork()) == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        f = creat(argv[6], 0666);
        if (f < 0) {
            fprintf(stderr, "permission denied\n");
            exit(0);
        }
        dup2(f, 1);
        close(f);
        execlp(argv[3], argv[3], argv[5], NULL);
        fprintf(stderr, "exec in pr3 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "first fork failed\n");
        exit(0);
    }

    pid3 = pid;

    if ((pid = fork()) == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec in pr1 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "second fork failed\n");
        exit(0);
    }

    pid = wait(&st);
    if (pid == pid3) {
        fprintf(stderr, "error in pr3\n");
        exit(0);
    }

    if ((pid = fork()) == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        f = open(argv[4], O_RDONLY);
        if (f < 0) {
            fprintf(stderr, "permission denied\n");
            exit(0);
        }
        dup2(f, 0);
        close(f);
        execlp(argv[2], argv[2], NULL);
        fprintf(stderr, "exec in pr2 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "third fork failed\n");
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    wait(&st);
    wait(&st);

    return 0;
}
