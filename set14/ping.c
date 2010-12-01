#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

enum {
    NUM = 4,
};

int fd[2];
int sig;

pid_t g_pid[NUM];
pid_t s_pid1, s_pid2;

void send_receive(int pid) {
    int st, msg;
    st = read(fd[0], &msg, sizeof(int));
    printf("Pid: %d, got %d message\n", getpid(), msg);
    msg++;
    st = write(fd[1], &msg, sizeof(int));
    kill(pid, sig);
}

void sig_handler(int r_sig)
{
    if (r_sig == SIGINT) {
        int i;
        for (i = 0; i < NUM; i++) {
            kill(g_pid[i], SIGINT);
            wait(NULL);
        }
        exit(0);
    }
    if (r_sig == SIGUSR1) {
        send_receive(s_pid1);
        return;
    }
    if (r_sig == SIGUSR2) {
        send_receive(s_pid2);
        return;
    }
}

void infinity(void) {
    while (1)
        pause();
}

int main(void)
{
    int i, st;
    int msg = 0;

    pid_t pid;

    st = pipe(fd);

    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);

    i = 0;
    if ((pid = fork()) == 0) {
        sig = SIGUSR1;
        s_pid1 = getppid();
        infinity();
    }
    g_pid[i++] = pid;

    if ((pid = fork()) == 0) {
        sig = SIGUSR2;
        s_pid1 = getppid();
        infinity();
    }
    g_pid[i++] = pid;

    if ((pid = fork()) == 0) {
        sig = SIGUSR1;
        s_pid1 = g_pid[1];
        infinity();
    }
    g_pid[i++] = pid;
    s_pid1 = pid;

    if ((pid = fork()) == 0) {
        sig = SIGUSR1;
        s_pid1 = g_pid[0];
        infinity();
    }
    g_pid[i++] = pid;
    s_pid2 = pid;

    signal(SIGINT, sig_handler);

    sig = SIGUSR1;
    st = write(fd[1], &msg, sizeof(int));
    kill(s_pid1, sig);

    infinity();

    return 0;
}
