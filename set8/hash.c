#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum
{
    STR_SIZE = 16,
    HASH_SIZE = 178543,
    HASH_TEST_MASK = 0xf0000000,
    HASH_MASK = 0xfffffff,
    HASH_SHIFT_LEFT = 4,
    HASH_SHIFT_RIGHT = 24,
};

typedef struct _Node
{
    struct _Node *next;
    char key[STR_SIZE];
} Node;

typedef struct
{
    Node *table[HASH_SIZE];
} Hash;

//PJW hash function from Wikipedia The Free Encyclopedia
int hash_func(char *str)
{
    int hash, test, i;
    hash = test = 0;

    for (i = 0; str[i]; i++) {
        hash = (hash << HASH_SHIFT_LEFT) + (unsigned char)(str[i]);

        if ((test = hash & HASH_TEST_MASK) != 0) {
            hash = ((hash ^ (test >> HASH_SHIFT_RIGHT)) & HASH_MASK);
        }
    }
    return hash % HASH_SIZE;
}

Node *create_node(char *str)
{
    Node *tmp = (Node*) malloc(sizeof (Node));
    strcpy(tmp->key, str);
    tmp->next = NULL;
    return tmp;
}


void hash_add(Hash *h, char *str)
{
    int ind = hash_func(str);
    Node *p = h->table[ind];

    if (p == NULL) {
        h->table[ind] = create_node(str);
        return;
    }

    while (p) {
        if (strcmp(p->key, str) == 0) {
            return;
        } 
        if (p->next == NULL)
            break;
        p = p->next;
    }

    p->next = create_node(str);
}

int hash_find(Hash *h, char *str)
{
    int ind = hash_func(str);
    Node *p = h->table[ind];

    while (p) {
        if (strcmp(p->key, str) == 0)
            return 1;
        p = p->next;
    }
    return 0;
}

void hash_delete(Hash *h, char *str)
{
    int ind = hash_func(str);
    Node *p = h->table[ind];

    if (p == NULL)
        return;

    if (strcmp(p->key, str) == 0) {
        h->table[ind] = p->next;
        free(p);
    }

    while (p->next) {
        if (strcmp(p->next->key, str) == 0) {
            Node *tmp = p->next;
            p->next = p->next->next;
            free(tmp);
            return;
        }
        p = p->next;
    }
}

void *hash_init(void)
{
    Hash *h = (Hash*) malloc(sizeof (Hash));
    int i;

    for (i = 0; i < HASH_SIZE; i++)
        h->table[i] = NULL;

    return h;
}

int main(void)
{
//    FILE *f;
//    f = freopen("input", "r", stdin);

    Hash *h = hash_init();
    char buf[STR_SIZE], *p;
    int i;

    while (1) {
        p = fgets(buf, STR_SIZE, stdin);

        for (i = 0; buf[i] && buf[i] != '\n'; i++);
        buf[i] = '\0';

        if (buf[0] == '#') {
            break;
        } else if (buf[0] == '+') {
            hash_add(h, buf + 2);
        } else if (buf[0] == '-') {
            hash_delete(h, buf + 2);
        } else if (buf[0] == '?') {
            if (hash_find(h, buf + 2))
                printf("YES\n");
            else
                printf("NO\n");
        }
    }

    free(h);

    return 0;
}
