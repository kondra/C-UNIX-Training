#include <stdio.h>

const int MAXLEN = 1000;

void sort(int *a, int n)
{
    int i, j, temp, imin;
    for (i = 0; i < n; i++) {
        for (imin = i, j = i + 1; j < n; j++) {
            if (a[j] < a[imin]) {
                imin = j;
            }
        }
        temp = a[i];
        a[i] = a[imin];
        a[imin] = temp;
    }
}

int main(void)
{
    int a[MAXLEN];
    int n, i;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    sort(a, n);
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    return 0;
}
