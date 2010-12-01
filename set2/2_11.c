#include <stdio.h>

int main ()
{
    int k, i;
    char str[1000], c;
    scanf ("%s%d", str, &k);
    for (i = 0; str[i]; i++) {
        c = str[i] - k;
        if (c - 'A' < 0)
            c += 26;
        printf ("%c", c);
    }
    return 0;
}
