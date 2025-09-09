#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26

struct trie
{
    char value;
    struct trie *children[ALPHABET_SIZE];
    bool end_of_key;
};

struct trie *trie_create(char value); //
struct trie *trie_insert(struct trie *root, char *key);
struct trie *trie_lookup(struct trie *root, char *key);
struct trie *trie_delete(struct trie *root, char *key); 
void trie_print(struct trie *root);                 

struct trie *trie_create(char value)
{
    struct trie *node = malloc(sizeof(struct trie));
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    node->end_of_key = false;
    node->value = value;
    return node;
}

struct trie *trie_insert(struct trie *root, char *key)
{
    struct trie *temp = root;

    for (int i = 0; key[i] != '\0'; i++)
    {
        int index = key[i] - 'a';
        if (temp->children[index] == NULL)
        {
            temp->children[index] = trie_create(key[i]);
        }
        temp = temp->children[index];
    }
    temp->end_of_key = true;
    return root;
}

struct trie *trie_lookup(struct trie *root, char *key)
{
    struct trie *temp = root;

    for (int i = 0; key[i] != '\0'; i++)
    {
        int position = key[i] - 'a';
        if (temp->children[position] == NULL)
        {   
            printf("The word %s is not found!\n", key);
            return root;
        }

        temp = temp->children[position];
    }
    if (temp != NULL && temp->end_of_key == true)
    {
        printf("The word %s is found!\n", key);
        return root;
    }
    else
    {
        printf("The word %s is not found!\n", key);
        return root;
    }
}

void free_trienode(struct trie *node)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            free_trienode(node->children[i]);
        }
    }
    free(node);
}

struct trie *trie_delete(struct trie *root, char *key)
{
    struct trie *temp = root;

    for (int i = 0; key[i] != '\0'; i++)
    {
        int count = 0;
        int position = key[i] - 'a';
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (temp->children[i] != NULL)
            {
                count++;
            }
        }
        if (count == 1)
        {
            free_trienode(temp->children[position]);
            temp->children[position] = NULL;
            return root;
        }
        temp = temp->children[position];
    }
    return root;
}

void trie_print(struct trie *root)
{
    if (!root)
        return;
    struct trie *temp = root;
    printf("%c -> ", temp->value);
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        trie_print(temp->children[i]);
    }
}

int main()
{
    struct trie *root = trie_create('\0');

    root = trie_insert(root, "hello");
    root = trie_insert(root, "hi");
    root = trie_insert(root, "teabag");
    root = trie_insert(root, "teacan");
    root = trie_delete(root, "hello");
    trie_lookup(root, "tea");
    trie_lookup(root, "teabag");
    trie_lookup(root, "teacan");
    trie_lookup(root, "hi");
    trie_lookup(root, "hey");
    trie_lookup(root, "hello");

    trie_print(root);   
    return 0;
}
