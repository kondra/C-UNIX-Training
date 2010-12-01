#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i, j, k;
    int **a;

    if (scanf("%d", &n) == EOF)
        return 0;
    a = (int**) calloc(n, sizeof (int*));
    for (i = 0; i < n; i++)
        a[i] = (int*) calloc(n, sizeof (int));

    for (k = 1, j = 0; k; j += 2) {
        k = 0;
        for (i = j == 0 ? 0 : j - 1; i < n - j; i++, k++)
            a[j][i] = 1;
        for (i = j; i < n - j; i++, k++)
            a[i][n - j - 1] = 1;
        for (i = n - j - 1; i >= j; i--, k++)
            a[n - j - 1][i] = 1;
        for (i = n - j - 1; i > j + 1; i--, k++)
            a[i][j] = 1;
    }


    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d", a[i][j]);
        printf("\n");
    }

    for (i = 0; i < n; i++)
        free(a[i]);
    free(a);

    return 0;
}

