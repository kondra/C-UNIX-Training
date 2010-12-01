#include <stdio.h>
#include <stdlib.h>

struct _Node
{
    struct _Node *left, *right;
    int val;
};

typedef struct _Node Node;

Node *create_node(int val)
{
    Node *tmp = (Node *)malloc(sizeof (Node));
    tmp->left = tmp->right = NULL;
    tmp->val = val;
    return tmp;
}

Node *add(Node *root, int val)
{
    if (root == NULL)
        return create_node(val);

    if (root->val < val)
        root->left = add(root->left, val);
    else if (root->val > val)
        root->right = add(root->right, val);
    return root;
}

int calc(Node *root)
{
    if (root == NULL)
        return 0;

    return calc(root->right) + calc(root->left) + 1;
}

void destroy(Node *root)
{
    if (root == NULL)
        return;

    destroy(root->left);
    destroy(root->right);

    free(root);
}

int main(void)
{
//    FILE *f;
//    f = freopen("input", "r", stdin);

    Node *root = NULL;
    int cur, s;

    while (1) {
        s = scanf("%d", &cur);
        if (cur == 0)
            break;
        root = add(root, cur);
    }
    printf("%d\n", calc(root));
    destroy(root);

    return 0;
}
