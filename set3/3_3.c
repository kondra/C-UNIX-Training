#include <stdio.h>
#include <string.h>

const int TEMPLATE_NUM = 2;
const int TEMPLATE_LEN = 30;
const int STRING_LEN = 5000;

int is_alpha(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

int str_len(char *cs)
{
    int n = 0;
    while (cs[n++])
        ;
    return n - 1;
}

int is_valid_key(char *ct)
{
    int j;
    if (!is_alpha(ct[1]))
        return 0;
    for (j = 1; ct[j] != '%'; j++)
        if (!is_alpha(ct[j]) && !is_digit(ct[j]))
            return 0;
    return 1;
}

int main(void)
{
//    freopen ("input", "r", stdin);

    char key[TEMPLATE_NUM][TEMPLATE_LEN], value[TEMPLATE_NUM][TEMPLATE_LEN];
    char str[STRING_LEN];
    char res[STRING_LEN];
    int i, j, k, l, n;
    int flag;

    for (i = 0; i < TEMPLATE_NUM; i++) {
        scanf("%s%s\n", key[i], value[i]);
        if (!is_valid_key(key[i])) {
            printf("ERROR");
            return 0;
        }
    }
    fgets(str, STRING_LEN, stdin);
    n = str_len(str);

    for (k = 0, i = 0; i < n; i++) {
        if (str[i] == '%') {
            if (str[i + 1] == '%') {
                res[k++] = '%';
                i++;
                continue;
            }
            if (!is_valid_key(str + i)) {
                printf("ERROR");
                return 0;
            }
            for (flag = 0, j = 0; j < TEMPLATE_NUM; j++) {
                for (flag = 1, l = 1; str[i + l] != '%' && key[j][l] != '%'; l++) {
                    if (str[i + l] != key[j][l]) {
                        flag = 0;
                        break;
                    }
                }
                if (!flag)
                    continue;
                for (l = 0; value[j][l]; l++)
                    res[k++] = value[j][l];
                i += str_len(key[j]) - 1;
                break;
            }
            if (!flag) {
                res[k++] = '%';
                for (i++; str[i] != '%'; i++)
                    res[k++] = str[i];
                res[k++] = '%';
            }
            continue;
        }
        res[k++] = str[i];
    }
    res[k] = '\0';
    printf ("%s\n", res);

    return 0;
}
