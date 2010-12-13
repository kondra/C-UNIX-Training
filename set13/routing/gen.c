#include <stdio.h>

int main(int argc, char **argv)
{
    int fl, n, i;
    FILE *f;

    f = freopen("01.2.dat", "w", stdout);
    fl = 0;
    n = 100000;
    printf("%d %d %d ", 78, 112, n);
    for (i = 0; i < n; i++) {
        if (i % 2 == fl)
            printf("1 ");
        else
            printf("2 ");
    }
    fclose(f);

    f = freopen("01.1.dat", "w", stdout);
    fl = 1;
    n = 100000;
    printf("%d %d %d ", 79, 111, n);
    for (i = 0; i < n; i++) {
        if (i % 2 == fl)
            printf("1 ");
        else
            printf("2 ");
    }

    return 0;
}
