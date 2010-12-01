#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a)>(b)?(a):(b))

struct _Node
{
    struct _Node *left, *right;
    int rh, lh;
    int val;
};

typedef struct _Node Node;

Node *create_node(int val)
{
    Node *tmp = (Node *)malloc(sizeof (Node));
    tmp->left = tmp->right = NULL;
    tmp->lh = tmp->rh = 0;
    tmp->val = val;
    return tmp;
}

Node *add(Node *root, int val)
{
    if (root == NULL)
        return create_node(val);

    if (root->val < val) {
        root->left = add(root->left, val);
        root->lh = MAX(root->left->rh, root->left->lh) + 1;
    } else if (root->val > val) {
        root->right = add(root->right, val);
        root->rh = MAX(root->right->rh, root->right->lh) + 1;
    }
    return root;
}

int check(Node *root)
{
    if (root == NULL)
        return 1;
    return abs(root->lh - root->rh) <= 1 && check(root->right) && check(root->left);
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
    int cur, s, res;

    while (1) {
        s = scanf("%d", &cur);
        if (cur == 0)
            break;
        root = add(root, cur);
    }
    res = check(root);

    if (res)
        printf("YES");
    else
        printf("NO");

    destroy(root);

    return 0;
}
