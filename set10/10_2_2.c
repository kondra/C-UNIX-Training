#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

void next(int i, int n, int k)
{
    int st, pid;

    if (i == k)
        return;

    if ((pid = fork()) == 0) {
        int j;
        for (j = i * n; j < i * n + n - 1; j++)
            printf("%d ", j);
        printf("\n");
        next(i + 1, n, k);
        if (i != k - 1)
            wait(&st);
    }
}

int main(void)
{
    int k, n;
    int st, s;

     scanf("%d%d", &k, &n);

    next(0, n, k);
    wait(&st);

    return 0;
}
