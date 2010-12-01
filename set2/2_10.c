#include <stdio.h>

int main ()
{
    double res, f;
    int n, i;
    scanf ("%d", &n);
    for (f = 1.0, res = 1.0, i = 1; i <= n; i++, f *= i)
        res += 1.0/f;
    printf ("%.30lf", res);
    return 0;
}
