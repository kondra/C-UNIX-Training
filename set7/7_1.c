#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_SIZE = 1000;

struct _Node 
{
    struct _Node *left, *right;
    int val;
};

typedef struct _Node Node;

typedef struct 
{
    Node *head, *last;
    int size;
} Dequeue;

Node *create_node(int val, Node *left, Node *right)
{
    Node *tmp;
    tmp = (Node*) malloc(sizeof (Node));
    tmp->val = val;
    tmp->left = left;
    tmp->right = right;
    return tmp;
}

void destroy_node(Node *node)
{
    if (node == NULL)
        return;
    free(node);
}

Dequeue *create(void)
{
    Dequeue *tmp = (Dequeue*)malloc(sizeof (Dequeue));
    tmp->head = tmp->last = NULL;
    tmp->size = 0;
    return tmp;
}

void clear(Node *node)
{
    if (node == NULL)
        return;

    clear(node->left);
    destroy_node(node);
}

void destroy(Dequeue *dq)
{
    if (dq == NULL)
        return;

    if (dq->size != 0)
        clear(dq->head);
    free(dq);
}

void push_front(Dequeue *dq, int val)
{
    if (dq->head != NULL) {
        Node *tmp = create_node(val, NULL, dq->head);
        dq->head->left = tmp;
        dq->head = tmp;
    } else {
        dq->last = dq->head = create_node(val, NULL, NULL);
    }
    dq->size++;
}

void push_back(Dequeue *dq, int val)
{
    if (dq->last != NULL) {
        Node *tmp = create_node(val, dq->last, NULL);
        dq->last->right = tmp;
        dq->last = tmp;
    } else {
        dq->head = dq->last = create_node(val, NULL, NULL);
    }
    dq->size++;
}

int front(Dequeue *dq, int *err)
{
    if (dq->size == 0 || dq->head == NULL) {
        *err = 1;
        return -1;
    }
    *err = 0;
    return dq->head->val;
}

int back(Dequeue *dq, int *err)
{
    if (dq->size == 0 || dq->last == NULL) {
        *err = 1;
        return -1;
    }
    *err = 0;
    return dq->last->val;
}

int pop_front(Dequeue *dq, int *err)
{
    int res;
    Node *tmp;

    if (dq->size == 0 || dq->head == NULL) {
        *err = 1;
        return -1;
    }

    *err = 0;
    res = dq->head->val;
    dq->size--;
    tmp = dq->head;
    dq->head = dq->head->right;
    destroy_node(tmp);
    if (dq->size == 0)
        dq->head = dq->last = NULL;
    return res;
}

int pop_back(Dequeue *dq, int *err)
{
    int res;
    Node *tmp;

    if (dq->size == 0 || dq->last == NULL) {
        *err = 1;
        return -1;
    }

    *err = 0;
    res = dq->last->val;
    dq->size--;
    tmp = dq->last;
    dq->last = dq->last->left;
    destroy_node(tmp);
    if (dq->size == 0)
        dq->head = dq->last = NULL;
    return res;
}

int size(Dequeue *dq)
{
    return dq->size;
}

int main(void)
{
//    freopen("input", "r", stdin);

    char buf[MAX_SIZE], *fs;
    int err, s, d, i, res;
    Dequeue *dq = create();

    while (1) {
        fs = fgets(buf, MAX_SIZE, stdin);
        s = scanf("\n");

        for (i = 0; buf[i] != ' ' && buf[i] != '\n' && buf[i]; i++);
        buf[i] = '\0';

        if (strcmp(buf, "push_front") == 0) {
//            if (!isdigit(*(buf + i + 1))) { 
//                printf("error\n");
//                continue;
//            }
            d = atoi(buf + i + 1);
            push_front(dq, d);
            printf("ok\n");
        } else if (strcmp(buf, "push_back") == 0) {
//            if (!isdigit(*(buf + i + 1))) { 
//                printf("error\n");
//                continue;
//            }
            d = atoi(buf + i + 1);
            push_back(dq, d);
            printf("ok\n");
        } else if (strcmp(buf, "pop_front") == 0) {
            err = 0;
            res = pop_front(dq, &err);
            if (err != 0)
                printf("error\n");
            else
                printf("%d\n", res);
        } else if (strcmp(buf, "pop_back") == 0) {
            err = 0;
            res = pop_back(dq, &err);
            if (err != 0)
                printf("error\n");
            else
                printf("%d\n", res);
        } else if (strcmp(buf, "front") == 0) {
            err = 0;
            res = front(dq, &err);
            if (err != 0)
                printf("error\n");
            else
                printf("%d\n", res);
        } else if (strcmp(buf, "back") == 0) {
            err = 0;
            res = back(dq, &err);
            if (err != 0)
                printf("error\n");
            else
                printf("%d\n", res);
        } else if (strcmp(buf, "size") == 0) {
            printf("%d\n", size(dq));
        } else if (strcmp(buf, "clear") == 0) {
            dq->size = 0;
            dq->head = dq->last = NULL;
            clear(dq->head);
            printf ("ok\n");
        } else if (strcmp(buf, "exit") == 0) {
            printf("bye\n");
            break;
        }
    }

    destroy(dq);

    return 0;
}
