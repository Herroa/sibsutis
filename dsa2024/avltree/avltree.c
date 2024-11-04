#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node
{
    int key;
    char *value;
    struct node *left;
    struct node *right;
    int height;
};

int getHeight(struct node *n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int getBalanceFactor(struct node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

struct node *createnode(int key, char *value)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

struct node *rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;

    x->right = y;
    y->left = T2;

    x->height = max(getHeight(x->right), getHeight(x->left)) + 1;
    y->height = max(getHeight(y->right), getHeight(y->left)) + 1;

    return x;
}

struct node *leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->right), getHeight(x->left)) + 1;
    y->height = max(getHeight(y->right), getHeight(y->left)) + 1;

    return y;
}

struct node *avltree_add(struct node *node, int key, char *value)
{
    if (node == NULL)
        return createnode(key, value);

    if (key < node->key)
        node->left = avltree_add(node->left, key, value);
    else if (key > node->key)
        node->right = avltree_add(node->right, key, value);

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int bf = getBalanceFactor(node);

    // Left Left Case
    if (bf > 1 && key < node->left->key)
    {
        return rightRotate(node);
    }
    // Right Right Case
    if (bf < -1 && key > node->right->key)
    {
        return leftRotate(node);
    }
    // Left Right Case
    if (bf > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left Case
    if (bf < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct node *avltree_lookup(struct node *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (root->key == key)
    {
        return root;
    }

    if (key > root->key)
    {
        avltree_lookup(root->right, key);
    }
    else
    {
        avltree_lookup(root->left, key);
    }
    return NULL;
}

struct node *avltree_delete(struct node *root, int x)
{
    struct node *temp = NULL;

    if (root == NULL)
    {
        return NULL;
    }

    if (x > root->key)
    {
        root->right = avltree_delete(root->right, x);
        if (getBalanceFactor(root) == 2)
        {
            if (getBalanceFactor(root->left) >= 0)
            {
                root = rightRotate(root);
            }
            else
            {
                root->left = rightRotate(root->left);
                root = rightRotate(root);
            }
        }
    }
    else if (x < root->key)
    {
        root->left = avltree_delete(root->left, x);
        if (getBalanceFactor(root) == -2)
        {
            if (getBalanceFactor(root->right) <= 0)
            {
                root = rightRotate(root);
            }
            else
            {
                root->right = rightRotate(root->right);
                root = rightRotate(root);
            }
        }
    }
    else
    {
        if (root->right != NULL)
        {
            temp = root->right;
            while (temp->left != NULL)
                temp = temp->left;

            root->key = temp->key;
            root->right = avltree_delete(root->right, temp->key);
            if (getBalanceFactor(root) == 2)
            {
                if (getBalanceFactor(root->left) >= 0)
                {
                    root = rightRotate(root);
                }
                else
                {
                    root->left = rightRotate(root->left);
                    root = rightRotate(root);
                }
            }
        }
        else
        {
            return (root->left);
        }
    }
    root->height = getHeight(root);
    return root;
}

struct node *avltree_delete_lazy(struct node *root, int x){
    
}

struct node *avltree_min(struct node *tree)
{
    if (tree == NULL)
        return NULL;
    struct node *l = tree;
    while (l->left != NULL)
        l = l->left;
    return l;
}

struct node *avltree_max(struct node *tree)
{
    if (tree == NULL)
        return NULL;
    struct node *r = tree;
    while (r->right != NULL)
        r = r->right;
    return r;
}

void avltree_free(struct node *root)
{
    if (root != NULL)
    {
        avltree_free(root->left);
        free(root);
        avltree_free(root->right);
    }
}

void print_node(struct node *tree, int prob)
{
    if (tree != NULL)
    {
        print_node(tree->left, prob + 2);

        for (int n = 0; n < prob; n++)
        {
            printf("\t");
        }

        printf("%d\n", tree->key);
        print_node(tree->right, prob + 2);
    }
}

int main()
{
    struct node *root = NULL;
    struct node *lookup = NULL;
    double start;
    double end;
    for (int i = 1; i < 50000; i++)
    {
        root = avltree_add(root, i, "sss");
        if (i % 1000 == 0)
        {
            start = clock();
            for (int j = 0; j < 100000; j++)
                lookup = avltree_lookup(root, rand() % i);
            end = clock();
            printf("%.6lf\n", (end - start) / CLOCKS_PER_SEC);
        }
    }

    // avltree_delete(root, 4);
    // lookup = avltree_lookup(root, 2);
    avltree_free(root);
    return 0;
}