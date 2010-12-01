#include <stdio.h>

const int LEN = 200;

int main()
{
    char str[LEN], str2[LEN];
    int i, j, n;
    scanf ("%s", str);
    for (i = 0, j = 0; str[i]; i++) {
        if (str[i] == ' ')
            continue;
        str2[j++] = str[i];
    }
    n = j;
    for (i = 0; i <= n / 2; i++) {
        if (str[i] != str[n - i - 1]) {
            printf ("no");
            return 0;
        }
    }
    printf ("yes");
    return 0;
}
