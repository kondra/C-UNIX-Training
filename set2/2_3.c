#include <stdio.h>

int main (void)
{
    int a, i, j;
    scanf ("%d%d", &a, &i);
    i--;
    for (j = ~0; i >= 0; i--) {
        j ^= 1 << i;
        a &= j;
    }
    printf ("%d", a);
    return 0;
}
