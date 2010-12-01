#include <stdio.h>
#include <stdlib.h>

const int MAXLEN = 1010;

char *str_tok(char *str, char *delim)
{
    static char *saved = NULL;
    char *ans;
    int len, i, j, dlen;
    int flag = 0;

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
                //can I use flag here???
                flag = 1;
                break;
            }
        }
        if (flag)
            break;
        str++;
    }

    len = str - saved;
    if (len != 0) {
        //actually std strtok doesn't use malloc, it kills input string by inserting '\0'
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
//    freopen("input", "r", stdin);

    char str[MAXLEN], delim[MAXLEN], *cur;
    int i;

    fgets(str, MAXLEN, stdin);
    fgets(delim, MAXLEN, stdin);

    for(i = 0; delim[i] != '\n'; i++)
        ;
    delim[i] = '\0';
    for(i = 0; str[i] != '\n'; i++)
        ;
    str[i] = '\0';

    cur = str_tok(str, delim);
    while (cur) {
        printf("%s\n", cur);
        free(cur);
        cur = str_tok(NULL, delim);
    }

    return 0;
}
