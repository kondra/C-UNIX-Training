#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

typedef void (*sighandler_t)(int);

volatile sig_atomic_t flag;

void sig_handler(int sig)
{
    if (sig == SIGALRM)
        flag = 1;
}

void mysleep(unsigned int seconds)
{
    unsigned int save;
    sighandler_t prev;

    prev = signal(SIGALRM, sig_handler);
    save = alarm(seconds);

    while (1) {
        pause();
        if (flag)
            break;
    }

    if (save - seconds > 0)
        alarm(save - seconds);
    signal(SIGALRM, prev);
}

int main(void)
{
    alarm(4);
    mysleep(2);
    printf("1\n");
    while(1)
        pause();

    return 0;
}
