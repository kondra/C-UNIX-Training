#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

enum {
    MAX_SIZE = 10,
    BASE = 10,
    PARAM = 3,
};

void error_handler(const char *name, const char *descr)
{
    printf("ERROR\n");
    fprintf(stderr, "pid: %d, name: %s, %s\n", getpid(), name, descr);
    exit(0);
}

int main(int argc, char *argv[])
{
    char *endptr;
    int num1, num2;
    int st;
    pid_t pid;

    if (argc != PARAM)
        error_handler(argv[0], "wrong number of parameters");

    num1 = strtol(argv[1], &endptr, BASE);
    if (*endptr != '\0')
        error_handler(argv[0], "wrong parameter");

    num2 = strtol(argv[2], &endptr, BASE);
    if (*endptr != '\0')
        error_handler(argv[0], "wrong parameter");

    if ((pid = fork()) == 0) {
        printf("%d %d %d\n", num1 + num2, getpid(), getppid());
        exit(0);
    } else if (pid < 0)
        error_handler(argv[0], "fork failed");

    if ((pid = fork()) == 0) {
        printf("%d %d %d\n", num1 * num2, getpid(), getppid());
        exit(0);
    } else if (pid < 0)
        error_handler(argv[0], "fork failed");

    wait(&st);
    wait(&st);

    return 0;
}
