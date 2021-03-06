#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, m, i, j;
    int k = 0;
    int **a;

    if (scanf("%d%d", &n, &m) == EOF)
        return 0;
    a = (int**) malloc(sizeof (int*) * n);
    for (i = 0; i < n; i++)
        a[i] = (int*) malloc(sizeof (int) * m);

    for (i = 0; i < m; i++) {
        for (j = 0; i - j >= 0 && j < n; j++)
            a[j][i - j] = k++;
    }

    for (i = 1; i < n; i++) {
        for (j = 0; i + j < n && m - j - 1 >= 0; j++)
            a[i + j][m - j - 1] = k++;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%3d", a[i][j]);
        printf("\n");
    }

    for (i = 0; i < n; i++)
        free(a[i]);
    free(a);

    return 0;
}
