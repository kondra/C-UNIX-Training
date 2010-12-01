#include <stdio.h>

int main ()
{
    int m, n, x, y;
    scanf ("%d%d%d%d", &m, &n, &x, &y);
    if (m == 1 && n == 1)
        return 0;
    if (m == 1) {
        if (y == 1) {
            printf ("%d %d\n", x, y + 1);
            return 0;
        }
        if (y == n) {
            printf ("%d %d\n", x, y - 1);
            return 0;
        }
        printf ("%d %d\n", x, y - 1);
        printf ("%d %d\n", x, y + 1);
        return 0;
    }
    if (n == 1) {
        if (x == 1) {
            printf ("%d %d\n", x + 1, y);
            return 0;
        }
        if (x == m) {
            printf ("%d %d\n", x - 1, y);
            return 0;
        }
        printf ("%d %d\n", x + 1, y);
        printf ("%d %d\n", x - 1, y);
        return 0;
    }
    if (x == 1) {
        if (y == 1) {
            printf ("%d %d\n", x + 1, y);
            printf ("%d %d\n", x, y + 1);
            return 0;
        }
        if (y == n) {
            printf ("%d %d\n", x + 1, y);
            printf ("%d %d\n", x, y - 1);
            return 0;
        }
        printf ("%d %d\n", x, y - 1);
        printf ("%d %d\n", x, y + 1);
        printf ("%d %d\n", x + 1, y);
        return 0;
    }
    if (x == m) {
        if (y == 1) {
            printf ("%d %d\n", x - 1, y);
            printf ("%d %d\n", x, y + 1);
            return 0;
        }
        if (y == n) {
            printf ("%d %d\n", x - 1, y);
            printf ("%d %d\n", x, y - 1);
            return 0;
        }
        printf ("%d %d\n", x - 1, y);
        printf ("%d %d\n", x, y + 1);
        printf ("%d %d\n", x, y - 1);
        return 0;
    }
    if (y == 1) {
        printf ("%d %d\n", x - 1, y);
        printf ("%d %d\n", x + 1, y);
        printf ("%d %d\n", x, y + 1);
        return 0;
    }
    if (y == n) {
        printf ("%d %d\n", x - 1, y);
        printf ("%d %d\n", x + 1, y);
        printf ("%d %d\n", x, y - 1);
        return 0;
    }
    printf ("%d %d\n", x - 1, y);
    printf ("%d %d\n", x + 1, y);
    printf ("%d %d\n", x, y - 1);
    printf ("%d %d\n", x, y + 1);
    return 0;
}
