#include <stdio.h>

int main ()
{
    char str[1000];
    int i;
    scanf ("%s", str);

    for (i = 0; str[i]; i++);
    for (i--; str[i] == '0'; i--);
    for (; i >= 0; i--)
        printf ("%c", str[i]);

    return 0;
}
