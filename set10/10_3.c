#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int pid, st;
    if ((pid = fork()) == 0) {
        execlp("ls", "ls", "-All", NULL);
        printf("error\n");
    } else if (pid == -1) {
        printf("error\n");
    }
    wait(&st);
    return 0;
}
