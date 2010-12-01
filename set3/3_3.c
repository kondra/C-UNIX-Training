#include <stdio.h>
#include <string.h>

const int TEMPLATE_NUM = 2;
const int TEMPLATE_LEN = 22;

enum
{
    TEXT = 1,
    FIRST_LETTER,
    TEMPLATE
};

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

int main(void)
{
    char key[TEMPLATE_NUM][TEMPLATE_LEN], value[TEMPLATE_NUM][TEMPLATE_LEN];
    char buf[TEMPLATE_LEN];
    char c;
    int state, matched;
    int i, j, curpos;

//    freopen("input", "r", stdin);

    for (i = 0; i < TEMPLATE_NUM; i++) {
        getchar();
        for (j = 0; (c = getchar()) != '%'; j++)
            key[i][j] = c;
        key[i][j] = '\0';
        getchar();
        scanf("%s\n", value[i]);
        if (!is_alpha(key[i][0])) {
            printf("ERROR");
            return 0;
        }
        for (j = 0; key[i][j]; j++) {
            if (!(is_alpha(key[i][j]) || is_digit(key[i][j]))) {
                printf("ERROR");
                return 0;
            }
        }
    }

    state = TEXT;
    while ((c = getchar()) != EOF) {
        if (c == '\n')
            break;
        if (state == TEXT) {
            if (c == '%') {
                state = FIRST_LETTER;
                continue;
            }
            putchar(c);
            continue;
        }
        if (state == FIRST_LETTER) {
            if (c == '%') {
                putchar('%');
                state = TEXT;
                continue;
            }
            if (!is_alpha(c)) {
                printf("ERROR");
                return 0;
            }
            curpos = 0;
            buf[curpos++] = c;
            state = TEMPLATE;
            continue;
        }
        if (state == TEMPLATE) {
            if (c == '%') {
                buf[curpos] = '\0';
                for (i = 0; i < TEMPLATE_NUM; i++) {
                    for (matched = 0, j = 0; buf[j] == key[i][j]; j++) {
                        if (buf[j] == '\0') {
                            matched = 1;
                            break;
                        }
                    }
                    if (matched) {
                        printf("%s", value[i]);
                        break;
                    }
                }
                if (!matched)
                    printf("%%%s%%", buf);
                state = TEXT;
                continue;
            }
            if (!(is_alpha(c) || is_digit(c))) {
                printf("ERROR");
                return 0;
            }
            if (curpos > TEMPLATE_LEN) {
                printf("ERROR");
                return 0;
            }
            buf[curpos++] = c;
            continue;
        }
    }
    if (state != TEXT) {
        printf("ERROR");
        return 0;
    }

    return 0;
}
