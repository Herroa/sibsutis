#include <stdio.h>
#include <stdlib.h>
void init(int** arr, int n)
//add a pointer
{
    *arr = malloc(n * sizeof(int));
    //dereferencing
    int i;
    for (i = 0; i < n; ++i)
    {
        (*arr)[i] = i;
    //dereferencing
    }
}
int main()
{
    int* arr = NULL;
    int n = 10;

    init(&arr, n);
    //transfer the address of arr
    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%d\n", arr[i]);
    }
    return 0;
}
