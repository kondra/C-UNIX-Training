#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum
{
    MAX_SIZE = 20
};

typedef enum
{
    BLACK = 0,
    RED,
} Colour;

struct _Tree
{
    struct _Tree *left, *right, *parent;
    char key[MAX_SIZE];
};

typedef struct _Tree Tree;

Tree *create_node(char *str)
{
    Tree *node = (Tree*) malloc(sizeof (Tree));
    strcpy(node->key, str);
    node->left = node->right = NULL;
    return node;
}

void left_rotate


void print_tree(Tree *root)
{
    if (root == NULL)
        return;

    print_tree(root->left);
    printf("%s\n", root->key);
    print_tree(root->right);
}

void destroy_tree(Tree *root)
{
    if (root == NULL)
        return;

    destroy_tree(root->left);
    destroy_tree(root->right);

    free(root);
}

int main(void)
{
    FILE *f;
    f = freopen("input", "r", stdin);

    Tree *tree = NULL;
    char buf[MAX_SIZE], *p;
    int i;

    while (1) {
        p = fgets(buf, MAX_SIZE, stdin);

        for (i = 0; buf[i] && buf[i] != '\n'; i++);
        buf[i] = '\0';

        if (buf[0] == '#') {
            break;
        } else if (buf[0] == '+') {
            tree = add_node(tree, buf + 2);
        } else if (buf[0] == '-') {
            tree = del_node(tree, buf + 2);
        } else if (buf[0] == '?') {
            if (find(tree, buf + 2) == NULL)
                printf("NO\n");
            else
                printf("YES\n");
        }
    }

    destroy_tree(tree);

    return 0;
}
