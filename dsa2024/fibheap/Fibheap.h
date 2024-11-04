#include <string>
#include <bits/stdc++.h>
#include <cmath>
#include <limits>

class FibNode
{
public:
    FibNode(int key, int value) : degree(0), mark(false), value(value), key(key), parent(nullptr), child(nullptr), left(nullptr), right(nullptr)
    {
    }
    int degree;
    bool mark;
    int value;
    int key;
    FibNode *parent;
    FibNode *child;
    FibNode *left;
    FibNode *right;
};

class FibHeap
{
public:
    int nnodes;
    FibNode *root;
    FibHeap() : nnodes(0), root(nullptr) {}

    void insert(int key, int value)
    {
        FibNode *node= new FibNode(key,value);
        node->left = node;
        node->right = node;
        AddNodeToRootList(node, root);
        if (root == NULL || node->key < root->key)
            root = node;
        nnodes++;
    };

    void fibunion(FibHeap* root2)
    {
        // FibHeap* newheap = new FibHeap;
        // newheap->root = root;
        if (root == nullptr || root2->root == nullptr)
        {
            if (root2->root == nullptr)
                return;
            else
            {
                root = root2->root;
            }
        }
        else
        {
            FibNode *rnode = root->right;
            FibNode *lnode = root2->root->left;
            root->right = root2->root;
            root2->root->left = root;
            rnode->left = lnode;
            lnode->right = rnode;
        }
        if (root2->root->key < root->key)
            root = root2->root;
        nnodes += root2->nnodes;
    }

    FibNode *min()
    {
        return root;
    }

    void consolidate()
    {
        int size = floor(log10(nnodes) / log10(2) + 1);;
        FibNode *A[size];
        FibNode *x = root;
        int d;
        for (int i = 0; i < size; i++)
            A[i] = nullptr;

        do
        {
            d = x->degree;
            while (A[d] != nullptr)
            {
                if (x->key > A[d]->key)
                    swap(x, A[d]);
                link(A[d], x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            x = x->right;
        } while(x != root);

        root = nullptr;
        for (int i = 0; i < size; i++){
            if (A[i] != nullptr){
                //AddNodeToRootList(A[i], root);
                if (root == nullptr || A[i]->key < root->key)
                    root = A[i];
            }
        }
    }

   FibNode* delete_min()
    {
        if (!root)
            return nullptr;
        FibNode *child = root->child; 
        FibNode *w = root->child;
        if (child){
            do{
                AddNodeToRootList(w, root);
                w = w->right;
            } while(w != child);
        }
        removefromrootlist(root);
        if (root == root->right)
            root = nullptr;
        else{
            root = root->right;
            consolidate();
        }
        nnodes--;
        return root;
    }

    void DecreaseKey(FibNode* x, int newkey){
        if (newkey > x->key)
            return;
        x->key = newkey;
        FibNode* y = x->parent;
        if (y != nullptr && x->key < y->key){
            cut(x,y);
            cascadingcut(y);
        }
        if (x->key < root->key)
            root = x;
    }

    void fibdelete(FibNode* x){
        DecreaseKey(x, INT_MIN);
        delete_min();
    }

protected:

    void AddNodeToRootList(FibNode* node, FibNode* _root)
    {
        if (_root == nullptr)
            return;
        if (_root->left == root)
        {
            _root->left = node;
            _root->right = node;
            node->right = root;
            node->left = root;
        }
        else
        {
            FibNode *lnode = _root->left;
            _root->left = node;
            node->right = root;
            node->left = lnode;
            lnode->right = node;
        }
    }

    void removefromrootlist(FibNode *x)
    {
        FibNode *left = x->left;
        FibNode *right = x->right;

        left->right = right;
        right->left = left;
    }

    void cut(FibNode* x, FibNode* y){
        removefromrootlist(x);
        y->degree--;

        AddNodeToRootList(x, root);
        x->parent = nullptr;
        x->mark = false;
    }

    void cascadingcut(FibNode* y){
        FibNode* z = y->parent;
        if (!z)
            return;
        if (y->mark == false)
            y->mark = true;
        else{
            cut(y,z);
            cascadingcut(z);
        }
    }

    void link(FibNode *x, FibNode *y)
    {
        x->degree++;
        removefromrootlist(y);
        y->parent = x;
        AddNodeToRootList(y, x->child);
        y->mark = false;
    }

    void swap(FibNode* x, FibNode* y)
    {
        int value1 = x->value;
        int key1 = x->key;
        x->value = y->value;
        x->key = y->key;
        y->value = value1;
        y->key = key1;
    }
};
