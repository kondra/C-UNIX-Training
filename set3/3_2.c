#include <stdio.h>

const int LEN = 100;

void array_modify (int *inp, int n, int k)
{
    int m, i, j, tmp;
    m = k / n;
    for (i = 1; i <= m; i++) {
        for (j = n * (i - 1); j < n * (i - 1) + n / 2; j++) {
            tmp = inp[j];
            inp[j] = inp[n * i - (j - n * (i - 1)) - 1];
            inp[n * i - (j - n * (i - 1)) - 1] = tmp;
        }
    }
}

int main(void)
{
    int i, n, k;
    int arr[LEN];
    scanf("%d%d", &k, &n);
    for (i = 0; i < k; i++)
        arr[i] = i + 1;
    array_modify (arr, n, k);
    for (i = 0; i < k; i++)
        printf ("%d ", arr[i]);
    return 0;
}
