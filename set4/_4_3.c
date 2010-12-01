#include <stdio.h>
#include <stdlib.h>

#include <string.h>

const int MAXLEN = 1010;

char *str_tok(char *str, char *delim)
{
    static char *saved = NULL;
    char *ans;
    int len, i, j, dlen;

    if (str == NULL)
        str = saved;
    else
        saved = str;

    for (dlen = 0; delim[dlen]; dlen++)
        ;

    while (*str) {
        len = str - saved;
        for (j = 0; j < dlen; j++) {
            if (*str == delim[j]) {
                if (len == 0) {
                    saved++;
                    break;
                }
                //actually std strtok doesn't use malloc, it kills input string by inserting '\0'
                ans = (char*) malloc(sizeof (char) * len);
                for (i = 0; i < len; i++)
                    ans[i] = saved[i];
                ans[i] = '\0';
                saved = str;
                return ans;
            }
        }
        str++;
    }

    len = str - saved;
    if (len != 0) {
        ans = (char*) malloc(sizeof (char) * len);
        for (i = 0; i < len; i++)
            ans[i] = saved[i];
        ans[i] = '\0';
        saved = str;
        return ans;
    }

    return NULL;
}

int main(void)
{
    freopen("input", "r", stdin);

    char str[MAXLEN], delim[MAXLEN], *cur;
    int i;

    fgets(str, MAXLEN, stdin);
    fgets(delim, MAXLEN, stdin);

    for(i = 0; delim[i] != '\n'; i++)
        ;
    delim[i] = '\0';
    int l1 = i;
    for(i = 0; str[i] != '\n'; i++)
        ;
    str[i] = '\0';
    int l2 =i;

    char *tcur;
    char test[MAXLEN];
    while (1) {
        printf ("%s - %s", str, delim);

        strcpy(test, str);
        tcur = strtok(test, delim);

        cur = str_tok(str, delim);
        while (cur) {
            if (strcmp(cur, tcur) != 0) {
                printf("cmp ERROR\n");
                printf("%s\n", cur);
                printf(" - %s\n", tcur);
                return 0;
            }
            printf("%s\n", cur);
            printf(" - %s\n", tcur);
            free(cur);

            cur = str_tok(NULL, delim);
            tcur = strtok(NULL, delim);
        }
        for (i = 0; i < l2; i++)
            str[i] = 'a' + rand() % 25;
        for (i = 0; i < l1; i++)
            delim[i] = 'a' + rand() % 25;
    }
    return 0;
}
