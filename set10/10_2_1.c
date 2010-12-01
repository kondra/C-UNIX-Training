#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int i, j, k, n, st, pid, s;
    int failed = 0;

    s = scanf("%d%d", &k, &n);

    for (i = 0; i < k; i++) {
        if ((pid = fork()) == 0) {
            for (j = i * n; j < i * n + n - 1; j++)
                printf("%d ", j);
            printf("\n");
            exit(0);
        } else if (pid < 0)
            failed++;
    }

    for (i = 0; i < k - failed; i++)
        wait(&st);

    return 0;
}
