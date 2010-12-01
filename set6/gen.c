#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f;
    f = freopen("input", "w", stdout);
    int i, k = 0;
    char c;
    for (i = 0; i < 100; i++) {
        if (k > 20) {
            printf("\n");
            k = 0;
        }
        c = rand();
        if (c % 67 == 0) {
            printf(" ");
        }
        printf("%c", 'a' + c % ('z'-'a') );
        k++;
    }
}
