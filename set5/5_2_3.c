#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, m, i, j, sz;
    int k = 1;
    int **a;

    if (scanf("%d", &sz) == EOF)
        return 0;
    n = 2 * sz + 1;
    m = 2 * sz + 1;
    a = (int**) calloc(n, sizeof (int*));
    for (i = 0; i < n; i++)
        a[i] = (int*) calloc(m, sizeof (int));

    i = j = sz;
    while (i >= 0 && j >= 0 && i < n && j < m) {
        do {
            a[i][j] = k++;
            i--;
        } while (i >= 0 && a[i][j - 1]);
        if (i < 0) break;

        do {
            a[i][j] = k++;
            j--;
        } while (j >= 0 && a[i + 1][j]);
        if (j < 0) break;

        do {
            a[i][j] = k++;
            i++;
        } while (i < n && a[i][j + 1]);
        if (i >= n) break;

        do {
            a[i][j] = k++;
            j++;
        } while (j < m && a[i - 1][j]);
        if (j >= m) break;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%3d", a[i][j] - 1);
        printf("\n");
    }

    for (i = 0; i < n; i++)
        free(a[i]);
    free(a);

    return 0;
}
