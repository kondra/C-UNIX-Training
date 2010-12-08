#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

enum {INT_LEN = CHAR_BIT * (sizeof(int) / sizeof(char))};

volatile sig_atomic_t num, ind;

void sig_handler(int sig)
{
    if (sig == SIGUSR1) {
        num <<= 1;
        num |= 1;
        ind++;
    }
    if (sig == SIGUSR2) {
        num <<= 1;
        ind++;
    }
}

int main(void)
{
    pid_t pid;

    //receive
    if ((pid = fork()) == 0) {
        ind = num = 0;

        signal(SIGUSR1, sig_handler);
        signal(SIGUSR2, sig_handler);

        while (ind < INT_LEN) {
            pause();
        }

        printf("%d\n", num);
        exit(0);
    }

    //send
    if (fork() == 0) {
        int mask, n, i;

        mask = 1 << (INT_LEN - 1);
        assert(scanf("%d", &n) == 1);
        for (i = 0; i < INT_LEN; i++) {
            if (n & mask) {
                kill(pid, SIGUSR1);
            }
            else  {
                kill(pid, SIGUSR2);
            }
            n <<= 1;

            usleep(20);
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}
