#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

char **parse(char *s)
{
    char **vect = (char**) malloc(sizeof (char*));
    char *p = NULL;
    int i = 1;

    while (1) {
        if ((*s == '\0' && p) || (*s == ' ' && p)) {
            vect = (char**) realloc(vect, sizeof (char*) * (i + 1));
            vect[i - 1] = (char*) malloc(sizeof(char) * (s - p + 1));
            strncpy(vect[i - 1], p, s - p);
            vect[i - 1][s - p + 1] = '\0';
            p = NULL;
            if (*s == '\0')
                break;
            i++;
        }
        if (*s == '\0')
            break;
        if (*s && *s != ' ' && p == NULL)
            p = s;
        s++;
    }
    i++;
    vect[i - 1] = NULL;

    return vect;
}

int _system(char *command)
{
    int pid, st;
    char **vect;
    vect = parse(command);

    if ((pid = fork()) == 0) {
        execvp(vect[0], vect);
        exit(-1);
    } else if (pid < 0) {
        free(vect);
        return -1;
    }

    free(vect);

    wait(&st);
    if (WIFEXITED(st))
        return WEXITSTATUS(st);
    else
        return -1;
}

int main(void)
{
    printf("%d\n", _system("ls -lah")); 
    return 0;
}
