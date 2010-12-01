#include <stdio.h>

int main ()
{
    int m, n, x, y;
    scanf ("%d%d%d%d", &m, &n, &x, &y);

    if (x - 1 > 0)
        printf ("%d %d\n", x - 1, y);
    if (x + 1 <= m)
        printf ("%d %d\n", x + 1, y);
    if (y - 1 > 0)
        printf ("%d %d\n", x, y - 1);
    if (y + 1 <= n)
        printf ("%d %d\n", x, y + 1);

    return 0;
}
