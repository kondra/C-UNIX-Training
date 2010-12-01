#include <stdio.h>

int main (void)
{
    int k, b, n;
    double x;
    scanf ("%d%d", &k, &b);
    n = k == 0 ? (b == 0 ? -1 : 0) : 1;
    x = k != 0 ? -(double)b/k : 0;
    printf ("%d %lf", n, x);
    return 0;
}
