#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int _system(const char *path)
{
    int pid, st;
    if ((pid = fork()) == 0) {
//        execlp(path, path, NULL);
        exit(-1);
    } else if (pid < 0)
        return -1;
    wait(&st);
    if (WIFEXITED(st))
        return WEXITSTATUS(st);
    else
        return -1;
}

int main(void)
{
    printf("%d\n", _system("ls")); 
    return 0;
}
