#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include"hashtable.h"
#define HASHTAB_SIZE 200003

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

unsigned int hashtab_hash(char *s)
{
    unsigned int h = 0, hash_mul = 31;
    while (*s)
        h = h * hash_mul + (unsigned int)*s++;
        //push pointer
    return h % HASHTAB_SIZE;
}

unsigned int AddHash(char *s)
{
    unsigned int h = 0;
    while (*s)
        h += (unsigned int)*s++;
    return h % HASHTAB_SIZE;
}

void hashtab_init(struct listnode **hashtab)
{
    int i;
    for (i = 0; i < HASHTAB_SIZE; i++)
        hashtab[i] = NULL;
}

void hashtab_add(struct listnode **hashtab, char *key, int value, int *count)
{
    struct listnode *node;
    node = malloc(sizeof(*node));
    int index = hashtab_hash(key);
    // int index = AddHash(key);
    if(hashtab[index] != NULL){
        node -> next = node;
        *count+=1;
    }
    ////
    if (node != NULL) {
        node->key = key;
        node->value = value;
        node->next = hashtab[index];
        hashtab[index] = node;
    }
    
}

struct listnode *hashtab_lookup(struct listnode **hashtab, char *key)
{
    struct listnode *node;

    int index = hashtab_hash(key);
    for (node = hashtab[index]; node != NULL; node = node->next) {
        if (0 == strcmp(node->key, key))
            return node;
        }
    return NULL;
}

void hashtab_delete(struct listnode **hashtab, char *key)
{
    struct listnode *node, *prev = NULL;
    int index = hashtab_hash(key);
    for (node = hashtab[index]; node != NULL; node = node->next) {
        if (0 == strcmp(node->key, key)) {
            if (prev == NULL)
            hashtab[index] = node->next;
            else
                prev->next = node->next;
            free(node);
            return;
    }
    prev = node;
    }
}

int main()
{
    FILE *file;
    if ((file = fopen("words.txt", "r"))==NULL) {
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


    struct listnode *hashtab[HASHTAB_SIZE];
    struct listnode *node;
    double t;
    int colisions = 0; int *p = &colisions;
    hashtab_init(hashtab);
    for(int i = 2;i<200001;i++){
        hashtab_add(hashtab, words[i], 17, p);
        if (i%10000==0){
            t = wtime();
            node = hashtab_lookup(hashtab, words[getrand(0, i - 1)]);
            t = wtime() - t;
            printf("%d %.6lf %d\n", i, t, colisions);
        }
    }
    ///
    // struct listnode *hashtab[HASHTAB_SIZE];
    // struct listnode *node;
    // hashtab_init(hashtab);
    // hashtab_add(hashtab, "Ocelot", 17);
    // hashtab_add(hashtab, "Flamingo", 4);
    // hashtab_add(hashtab, "Fox", 14);
    // node = hashtab_lookup(hashtab, "Flamingo");
    // if (node != NULL)
    // printf("Node: %s, %d\n", node->key, node->value);

    for(int i = 0;i<n;i++){
        free(words[n]);
    }
    free(words);
    return 0;
}