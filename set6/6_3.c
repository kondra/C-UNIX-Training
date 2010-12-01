#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct _Tree
{
    struct _Tree *left, *right;
    char *str;
    int count;
};

typedef struct _Tree Tree;

Tree *create_node(char *str)
{
    Tree *node = (Tree*) malloc(sizeof (Tree));
    node->count = 1;
    node->str = str;
    node->left = node->right = NULL;
    return node;
}

Tree *add_node(Tree *root, char *str)
{
    if (root == NULL)
        return create_node(str);

    if (strcmp(str, root->str) < 0) {
        root->left = add_node(root->left, str);
        return root;
    } else {
        root->right = add_node(root->right, str);
        return root;
    }
}

Tree *find(Tree *root, char *str)
{
    if (root == NULL)
        return NULL;

    int s = strcmp(str, root->str);
    if (s == 0)
        return root;
    if (s < 0)
        return find(root->left, str);
    else
        return find(root->right, str);
}

void print_tree(Tree *root)
{
    if (root == NULL)
        return;

    print_tree(root->left);
    printf("%s %d\n", root->str, root->count);
    print_tree(root->right);
}

void destroy_tree(Tree *root)
{
    if (root == NULL)
        return;

    destroy_tree(root->left);
    destroy_tree(root->right);

    free(root->str);
    free(root);
}

enum
{
    SAVE = 1,
    WAIT,
};

const int MAXLEN = 60;

int main(void)
{
//    if (freopen("input", "r", stdin) == NULL)
//        return 0;

    int state = WAIT;
    int i, j, c;
    char buf[MAXLEN], *tmp;
    Tree *root, *node;

    i = j = 0;
    root = node = NULL;

    while ((c = getchar()) != EOF) {
        if (state == WAIT) {
            if (!isspace(c)) {
                i = 0;
                buf[i++] = c;
                state = SAVE;
            }
            continue;
        }
        if (state == SAVE) {
            if (!isspace(c)) {
                buf[i++] = c;
                continue;
            }
            buf[i] = '\0';
            state = WAIT;
            
            if ((node = find(root, buf)) != NULL) {
                node->count++;
            } else {
                tmp = (char*) malloc(sizeof (char) * (i + 1));
                for (j = 0; buf[j]; j++)
                    tmp[j] = buf[j];
                tmp[j] = '\0';
                root = add_node(root, tmp);
            }
        }
    }

    print_tree(root);

    destroy_tree(root);

    return 0;
}
