#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum
{
    STR_SIZE = 16,
    HASH_SIZE = 1000,
};

enum
{
    FILLED = 0,
    FREE,
};

typedef struct
{
    char key[HASH_SIZE][STR_SIZE];
    int state[HASH_SIZE];
    int num;
}Hash;

//PJW hash function, from Wikipedia
int hash_func(char *str)
{
    int hash, test, i;
    hash = test = 0;

    for (i = 0; str[i]; i++) {
        hash = (hash << 4) + (unsigned char)(str[i]);

        if ((test = hash & 0xf0000000) != 0) {
            hash = ((hash ^ (test >> 24)) & (0xfffffff));
        }
    }
    return hash % HASH_SIZE;
}

Hash *hash_create(void)
{
    int i;
    Hash *h = (Hash*) malloc(sizeof (Hash));
    h->num = 0;
    for (i = 0; i < HASH_SIZE; i++) {
//        h->key[i] = NULL;
        h->state[i] = FREE;
    }
    return h;
}

void hash_destroy(Hash *h)
{
    //int i;
//    for (i = 0; i < HASH_SIZE; i++)
//        if (h->key[i] != NULL)
//            free(h->key[i]);
    free(h);
}

void hash_add(Hash *h, char *str)
{
    int i = hash_func(str);

//    if (i == 112)
//        printf("%s\n", str);

    while (1) {
//        if (h->key[i] == NULL) {
        if (h->state[i] == FREE) {
            if (h->num == HASH_SIZE - 1) {
                fprintf(stderr, "OLOLO\n");
                exit(0);
                return;
            }

            h->num++;

//            if (h->key[i] != NULL)
//                free(h->key[i]);

//            h->key[i] = (char*) malloc(sizeof (char) * (strlen(str) + 1));
            h->state[i] = FILLED;
            strcpy(h->key[i], str);

            if (strcmp(str, "w") == 0) {
                printf("ADD: %s - %d\n", h->key[i], i);
                printf("ADD: %d\n", h->state[i] == FILLED);
            }

            return;
        } else if (strcmp(h->key[i], str) == 0) {
            return;
        }

        i--;
        if (i < 0)
            i += HASH_SIZE;
    }
}

int hash_find(Hash *h, char *str)
{
    int i = hash_func(str);
    if (strcmp(str, "w") == 0) {
        printf("FIND: %s - %d\n", str, i);
        printf("KEY: %s - %d\n", h->key[i], hash_func(h->key[i]));
        printf("FLAG: %d\n", h->state[i] == FILLED);
    }

    while (1) {
//        if (h->key[i] == NULL) {
        if (h->state[i] == FILLED && strcmp(str, "w") == 0)
            printf("KEY: %s - %d\n", h->key[i], i);

        if (h->state[i] == FREE) {
            return -1;
        } else if (strcmp(h->key[i], str) == 0) {
            return i;
        }

        i--;
        if (i < 0)
            i += HASH_SIZE;
    }
    return -1;
}


void hash_delete(Hash *h, int ind)
{
    int i, r, j;
    j = i = ind;

    h->state[i] = FREE;
    h->num--;
    while (1) {
        i--;
        if (i < 0)
            i += HASH_SIZE;

        if (h->state[i] == FREE)
            return;

        r = hash_func(h->key[i]);
        if (r == i)
            continue;
        
        strcpy(h->key[j], h->key[i]);
        h->state[j] = FILLED;
        h->state[i] = FREE;
        j = i;
    }
}
int main(void)
{
    FILE *f;
    f = freopen("input", "r", stdin);

    Hash *h;
    char buf[STR_SIZE], *p;
    int i, ind;

    h = hash_create();

    int k = 0;
    while (1) {
        p = fgets(buf, STR_SIZE, stdin);

        for (i = 0; buf[i] && buf[i] != '\n'; i++);
        buf[i] = '\0';

        if (buf[0] == '#') {
            break;
        } else if (buf[0] == '+') {
            hash_add(h, buf + 2);
        } else if (buf[0] == '-') {
            if ((ind = hash_find(h, buf + 2)) != -1) {
                hash_delete(h, ind);
            }
        } else if (buf[0] == '?') {
            if (hash_find(h, buf + 2) == -1)
                printf("NO\n");
            else
                printf("YES\n");
        }
    }

    hash_destroy(h);

    return 0;
}
