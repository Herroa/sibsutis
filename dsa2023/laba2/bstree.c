#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "bstree.h"


double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min,int max)
{
    return (double)rand()/(RAND_MAX + 1.0) * (max - min) + min;
}

void tree_print(struct bstree * tree) {
    if (tree == NULL){} 
    else {
        tree_print(tree->left);
        printf("%s \n", tree->key); 
        tree_print(tree->right);
    }
}

struct bstree *bstree_create(char *key, int value)
{
    struct bstree *tmp = malloc(sizeof(struct bstree));
    if(tmp != NULL){
        tmp -> key = strdup(key);
        tmp -> value = value;
        tmp -> left = NULL;
        tmp -> right = NULL;
        tmp -> parent = NULL;
    }
    return tmp;
}

void bstree_add(struct bstree *tree, char *key, int value)
{
    if(tree == NULL){
        return;
    }
    struct bstree *parent, *node;
    while(tree != NULL){
        parent = tree;
        if(strcmp(tree -> key,key)>0){
            tree = tree -> left;
        }
        else if(strcmp(key,tree -> key)>0){
            tree = tree -> right;
        }
        else{
            return;
        }
    }
    node = bstree_create(key, value);
        if (strcmp(parent->key,key)>0){
            parent->left = node;
            node -> parent = parent;
        }
        else{
            parent->right = node;
            node -> parent = parent;
        }
    // printf("file added %s\n",key);
}

struct bstree *bstree_lookup(struct bstree *tree, char *key)
{

    while (tree != NULL) {
        if (strcmp(key,tree -> key) == 0)
            return tree;
        else if (strcmp(tree -> key,key))
            tree = tree->left;
        else
            tree = tree->right;
    }
    return tree;
}


struct bstree *bstree_min(struct bstree *tree, int *deep_pointer)
{
    if (tree == NULL)
        return NULL;
    struct bstree *l = tree;
    while (l -> left != NULL){
        l = l -> left;
        *deep_pointer+=1;
    }
    return l;
}


struct bstree *bstree_max(struct bstree *tree)
{
    if (tree == NULL)
        return NULL;
    struct bstree *r = tree;
    while (r -> right != NULL)
        r = r -> right;
    return r;
}

// struct bstree *bstree_delete(struct bstree* tree, char* key)
// {
//     struct bstree* parent = NULL;
//     struct bstree* node = bstree_lookup(tree,key);
//     parent = node -> parent;
//     if (node == NULL) {
//         return NULL;
//     }
//     // Случай 1: удаляемый узел не имеет дочерних элементов, т. е. является листовым узлом
//     if (node->left == NULL && node->right == NULL)
//     {
//         if (node != tree)
//         {
//             if (parent->left == node) {
//                 parent->left = NULL;
//             }
//             else {
//                 parent->right = NULL;
//             }
//         }
//         else {
//             tree = NULL;
//         }
//         free(node);
//     }
//     // Случай 2: удаляемый узел имеет двух потомков
//     else if (node->left && node->right)
//     {
//         struct bstree* successor = bstree_min(node->right);
//         char *val = successor->key;
//         bstree_delete(tree, successor->key);
//         node->key = val;
//     }
//     // Случай 3: удаляемый узел имеет только одного потомка
//     else {
//         struct bstree* child = (node->left)? node->left: node->right;
//         if (node != tree)
//         {
//             if (node == parent->left) {
//                 parent->left = child;
//             }
//             else {
//                 parent->right = child;
//             }
//         }
//         else {
//             tree = child;
//         }
//         free(node);
//     }
//     return tree;
// }




int main()
{

    // printf("%d\t%s\n",bstree_min(tree)->value,bstree_min(tree)->key);
    // printf("%d\t%s\n",bstree_max(tree)->value,bstree_max(tree)->key);
    FILE *file;
    if ((file = fopen("sorted_words", "r"))==NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    char **words = (char**)malloc(sizeof(char*));
    int n = 0;
    while (!feof(file))
    {
        words[n] = (char*)malloc(sizeof(char)* 256);
        fgets(words[n], 256, file);
        n++;
        words = (char**)realloc(words, sizeof(char*)*(n + 1));
    }
    fclose(file);


    struct bstree *tree = NULL;
    struct bstree *node = NULL;
    double t; int deep = 0; int *deep_pointer = &deep;
    tree = bstree_create("zzz", 100);
    for(int i = 2;i<200001;i++){
        // bstree_add(tree, words[getrand(0, i - 1)], 1);
        bstree_add(tree, words[200000 - i], 1);
        /////
        if (i%10000==0){
            t = wtime();
            bstree_min(tree, deep_pointer);
            // node = bstree_lookup(tree, words[getrand(0, i - 1)]);
            /////
            t = wtime() - t;
            printf("%d %.6lf,%d\n", i, t,deep);
            *deep_pointer = 0;
        }
    }    
    // tree_print(tree);
    for(int i = 0;i<n;i++){
        free(words[n]);
    }
    free(words);
    return 0;
}