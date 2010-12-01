#include <stdio.h>

const int MAXLEN = 1010;


int main(void)
{
    char str[MAXLEN];
    int i, pos;
    int cur_len = 0;
    int max_len = 0;
    int cur_pos = 0;

    fgets(str, MAXLEN, stdin);
    for (i = 0; str[i]; i++) {
        if (str[i] == '\n')
            break;
        if (str[i] != ' ') {
            cur_len++;
        } else {
            if (cur_len > max_len) {
                max_len = cur_len;
                pos = cur_pos;
            }
            cur_pos = i + 1;
            cur_len = 0;
        }
    }
    if (cur_len > max_len) {
        max_len = cur_len;
        pos = cur_pos;
    }
    for (i = pos; i < pos + max_len; i++)
        printf ("%c", str[i]);
    printf ("\n%d\n", max_len);
    return 0;
}
