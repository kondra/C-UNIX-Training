#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int st, pid;
    int fd1[2], fd2[2];

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(0);
    }

    //pr4
    if ((pid = fork()) == 0) {
        dup2(fd2[0], 0);
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        close(fd1[1]);
        execlp(argv[4], argv[4], NULL);
        fprintf(stderr, "exec in pr4 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "first fork failed\n");
        exit(0);
    }

    //pr1
    if ((pid = fork()) == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec in pr1 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "second fork failed\n");
        exit(0);
    }

    //pr2
    if ((pid = fork()) == 0) {
        dup2(fd2[1], 1);
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

    //pr3
    if ((pid = fork()) == 0) {
        dup2(fd2[1], 1);
        dup2(fd1[0], 0);
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        close(fd1[1]);
        execlp(argv[3], argv[3], NULL);
        fprintf(stderr, "exec in pr3 failed\n");
        exit(0);
    } else if (pid == -1) {
        fprintf(stderr, "fourth fork failed\n");
        exit(0);
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    wait(&st);
    wait(&st);
    wait(&st);
    wait(&st);

    return 0;
}
