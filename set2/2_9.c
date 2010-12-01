#include <stdio.h>

int main ()
{
    char str[20];
    int i, j, n;
    scanf ("%s", str);
    for (j = 1, i = 0; str[i]; i++)
        j <<= 1;
    for (j >>= 1, n = 0, i = 0; str[i]; i++, j >>= 1)
        n += j * (str[i] - '0');
    printf ("%d", n);
    return 0;
}
