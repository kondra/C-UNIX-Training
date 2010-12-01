#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int len, rlen;
    char **val;
} Stack;

const int MIN_SIZE = 16;
const int MAX_LEN = 128;
const int MAX_PATH_LEN = 1024;

void push(Stack *st, char *str)
{
    if (st->len == st->rlen) {
        st->val = (char**) realloc(st->val, sizeof (char*) * st->rlen * 2);
        st->rlen *= 2;
    }

    st->val[st->len] = (char*) realloc(st->val[st->len], sizeof (char) * strlen(str));
    strcpy(st->val[st->len], str);
    st->len++;
}

char *pop(Stack *st)
{
    if (st->len == 0)
        return NULL;
    return st->val[st->len--];
}

Stack *init(void)
{
    Stack *st = (Stack*) malloc (sizeof (Stack));
    st->len = 0;
    st->rlen = MIN_SIZE;
    st->val = (char**) malloc(sizeof (char*) * MIN_SIZE);
    return st;
}

void destroy(Stack *st)
{
    int i;
    for (i = 0; i < st->rlen; i++)
        if (st->val[i] != NULL)
            free(st->val[i]);
    free(st->val);
    st->len = st->rlen = 0;
}

void print_stack(Stack *st)
{
    int i;
    for (i = 0; i < st->len; i++)
        printf("%s\n", st->val[i]);
}

int check_abs(char *path, char *name)
{
    if (path[0] != '/') {
        fprintf(stderr, "ERROR: %s path is not absolute\n", name);
        printf("ERROR\n");
        return -1;
    }
    return 0;
}

int check_path(char *path, char *name)
{
    if (path[0] == '~' && (path[1] == '/' || path[1] == '\0'))
        return 0;
    if (path[0] == '.' && (path[1] == '/' || path[1] == '\0'))
        return 0;
    if (path[0] == '/')
        return 0;
    fprintf(stderr, "ERROR: %s is incorrect\n", name);
    printf("ERROR\n");
    return -1;
}

void push_path(Stack *st, char *path)
{
    int i, j;
    int len = strlen(path);
    for (i = 0; i < len; i++) {
        if (path[i] == '/')
            continue;
        if (path[i] == '.' && (path[i + 1] == '/' || path[i + 1] == '\0'))
            continue;
        if (path[i] == '.' && path[i + 1] == '.' && (path[i + 2] == '/' || path[i + 2] == '\0')) {
            i++;
            pop(st);
            continue;
        }
        for (j = i; path[j] && path[j] != '/'; j++);
        path[j] = '\0';
        push(st, path + i);
        path[j] = '/';
        i = j;
    }
}

void process(Stack *st, char *path, char *cur, char *home)
{
    if (path[0] == '/') {
        push_path(st, path);
        return;
    }
    if (path[0] == '~' && (path[1] == '/' || path[1] == '\0')) {
        push_path(st, home);
        push_path(st, path + 1);
        return;
    }
    if (path[0] == '.' && (path[1] == '/' || path[1] == '\0')) {
        push_path(st, cur);
        push_path(st, path);
        return;
    }
    exit(-1);
}

void chomp(char *str)
{
    while (*str != '\n')
        str++;
    *str = '\0';
}

int main(void)
{
//    if (freopen("input", "r", stdin) == NULL)
//        return 0;

    int i;
    char home[MAX_LEN], cur[MAX_LEN], path1[MAX_PATH_LEN], path2[MAX_PATH_LEN];
    Stack *st1 = init();
    Stack *st2 = init();

    if (fgets(home, MAX_LEN, stdin) == NULL)
        return 0;
    if (fgets(cur, MAX_LEN, stdin) == NULL)
        return 0;
    if (fgets(path1, MAX_PATH_LEN, stdin) == NULL)
        return 0;
    if (fgets(path2, MAX_PATH_LEN, stdin) == NULL)
        return 0;

    chomp(home);
    chomp(cur);
    chomp(path1);
    chomp(path2);

    if (check_abs(home, "home") || check_abs(cur, "current directory"))
        return 0;
    if (check_path(path1, "path 1") || check_path(path2, "path 2"))
        return 0;

    process(st1, path1, cur, home);
    process(st2, path2, cur, home);

    if (st1->len != st2->len) {
        printf("NO\n");
        return 0;
    }

    for (i = 0; i < st1->len; i++) {
        if (strcmp(st1->val[i], st2->val[i])) {
            printf("NO\n");
            return 0;
        }
    }

    printf("YES\n");

    destroy(st1);
    destroy(st2);

    return 0;
}
