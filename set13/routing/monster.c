#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int b, i, st;
    pid_t pid;

    st = read(0, &b, sizeof (int));
    for (i = 0; i < b; i++) {
        st = read(0, &pid, sizeof (pid_t));
        if (st < 0) {
            fprintf(stderr, "pipe closed before %d pids transmitted\n", b);
            exit(-1);
        }
        kill(pid, SIGKILL);
    }

    return 0;
}
