#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

enum { SIZE = 1 << 10 };

enum {
    CTRLC = 1,
    RCVD = 2,
    WAIT = 3,
};

volatile sig_atomic_t flag = 0;

void sig_handler(int sig)
{
    if (sig == SIGINT) {
        flag = CTRLC;
        return;
    }
    if (sig == SIGUSR1) {
        flag = WAIT;
        return;
    }
    if (sig == SIGUSR2) {
        flag = RCVD;
        return;
    }
}

int main(int argc, char **argv)
{
    int n, id, i, j;
    char *mem, *s;
    FILE *f;
    
    pid_t pid;

    n = strtol(argv[2], NULL, 10);

    f = fopen(argv[1], "r");
    id = shmget(IPC_PRIVATE, SIZE + 1, 0666);
    mem = (char*) shmat(id, 0, 0);

    signal(SIGINT, SIG_IGN);
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);

    if ((pid = fork()) == 0) {
        signal(SIGINT, sig_handler);

        while (1) {
            pause();
            if (flag == CTRLC) {
                j = 0;
                kill(getppid(), SIGUSR1);
            }
            if (flag == RCVD) {
                printf("%s", mem);
                for (i = 0; mem[i]; i++)
                    if (mem[i] == '\n')
                        j++;
                if (j < n)
                    kill(getppid(), SIGUSR1);
            }
        }

        exit(0);
    }

    while (1) {
        pause();
        if (flag == WAIT) {
            s = fgets(mem, SIZE, f);
            if (feof(f))
                break;
            kill(pid, SIGUSR2);
        }
    }

    shmdt(mem);
    fclose(f);
    kill(pid, SIGKILL);

    return 0;
}
