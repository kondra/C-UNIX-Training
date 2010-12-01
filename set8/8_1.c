#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {MAX_SIZE = 20};

struct _Tree
{
    struct _Tree *left, *right;
    char key[MAX_SIZE];
    int count;
};

typedef struct _Tree Tree;

Tree *create_node(char *str)
{
    Tree *node = (Tree*) malloc(sizeof (Tree));
    node->count = 1;
    strcpy(node->key, str);
    node->left = node->right = NULL;
    return node;
}

Tree *add_node(Tree *root, char *str)
{
    if (root == NULL)
        return create_node(str);

    int s = strcmp(str, root->key);
    if (s < 0)
        root->left = add_node(root->left, str);
    else if (s > 0)
        root->right = add_node(root->right, str);

    return root;
}

Tree *find(Tree *root, char *str)
{
    if (root == NULL)
        return NULL;

    int s = strcmp(str, root->key);
    if (s == 0)
        return root;
    if (s < 0)
        return find(root->left, str);
    else
        return find(root->right, str);
}

Tree *del_node(Tree *root, char *str)
{
    if (root == NULL)
        return NULL;

    int s = strcmp(str, root->key);
    if (s == 0) {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (root->right == NULL && root->left != NULL) {
            Tree *tmp = root->left;
            free(root);
            return tmp;
        }
        if (root->left == NULL && root->right != NULL) {
            Tree *tmp = root->right;
            free(root);
            return tmp;
        }

        Tree *cur = root->left;
        while (cur->right != NULL)
            cur = cur->right;
        strcpy(root->key, cur->key);

        root->left = del_node(root->left, root->key);
        return root;
    }
    if (s < 0)
        root->left = del_node(root->left, str);
    else if (s > 0)
        root->right = del_node(root->right, str);
    return root;
}

void print_tree(Tree *root)
{
    if (root == NULL)
        return;

    print_tree(root->left);
    printf("%s %d\n", root->key, root->count);
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
