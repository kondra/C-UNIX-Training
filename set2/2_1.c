#include <stdio.h>

int main (void)
{
    int a, i;
    scanf ("%d%d", &a, &i);
    a ^= 1 << i;
    printf ("%d", a);
    return 0;
}
