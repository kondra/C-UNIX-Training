#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
    FILE *f;
    f = freopen("input", "w", stdout);

    int i, j, s, t;

    srand(time(NULL));

    for (i = 0; i < 1000000; i++) {
        s = rand() % 3;ksdjfk;

//        if (s == 0)
            printf("+ ");
//        if (s == 1)
//            printf("? ");
//        if (s == 2)
//            printf("- ");

        s = rand() % 10;
        if (s == 0)
            s = 1;
        for(j = 0; j < s; j++) {
            t = rand() % ('z'-'a');
            printf("%c", 'a' + t);
        }
        printf("\n");
    }
    printf("#\n");

    return 0;
}
